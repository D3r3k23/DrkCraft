#include "Audio.hpp"

#include "AlTools.hpp"
#include "Core/Profiler.hpp"

#include <AL/al.h>
#include <AL/alc.h>

#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <cstdlib>

namespace DrkCraft
{
    static const float MASTER_LISTENER_GAIN = 0.25f; // -> 0.5 after volume ctrl added

    ////////////////////////////
    //       Mp3Decoder       //
    ////////////////////////////

    Mp3Decoder::Mp3Decoder(void)
    {
        DRK_LOG_CORE_TRACE("Initializing Mp3Decoder");
        mp3dec_init(&m_decoder);
    }

    mp3dec_t& Mp3Decoder::get(void)
    {
        return m_decoder;
    }

    /////////////////////////////
    //       Mp3FileInfo       //
    /////////////////////////////

    Mp3FileInfo::Mp3FileInfo(Mp3Decoder& decoder, const std::filesystem::path& filename)
    {
        mp3dec_file_info_t info;
        {
            DRK_PROFILE_SCOPE("Decode mp3");
            int result = mp3dec_load(&decoder.get(), filename.string().c_str(), &info, nullptr, nullptr);
            DRK_ASSERT_CORE(result == 0, "mp3dec_load may have failed");
        }
        m_size   = info.samples * sizeof(mp3d_sample_t);
        m_length  = m_size / (info.avg_bitrate_kbps * 1024.0f);
        m_buffer   = info.buffer;
        m_channels  = info.channels;
        m_sampleRate = info.hz;
    }

    Mp3FileInfo::~Mp3FileInfo(void)
    {
        free(m_buffer);
    }

    int16* Mp3FileInfo::get_buffer(void)
    {
        return static_cast<int16*>(m_buffer);
    }

    uint Mp3FileInfo::get_size(void) const
    {
        return m_size;
    }

    float Mp3FileInfo::get_length(void) const
    {
        return m_length;
    }

    uint Mp3FileInfo::get_channels(void) const
    {
        return m_channels;
    }

    uint Mp3FileInfo::get_sample_rate(void) const
    {
        return m_sampleRate;
    }

    /////////////////////////////
    //       AudioEngine       //
    /////////////////////////////

    AudioEngine::AudioEngine(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Initializing AudioEngine");

        int status = init_al();
        DRK_ASSERT_CORE(status == 0, "Failed to initialize OpenAL");

        ALfloat listenerPos[] = { 0.0f, 0.0f, 0.0f };
        ALfloat listenerVel[] = { 0.0f, 0.0f, 0.0f };
        ALfloat listenerOri[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

        alListenerfv(AL_POSITION, listenerPos);
        alListenerfv(AL_VELOCITY, listenerVel);
        alListenerfv(AL_ORIENTATION, listenerOri);

        set_gain(MASTER_LISTENER_GAIN);
    }

    AudioEngine::~AudioEngine(void)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("Stopping all playing audio sources");
        stop_all();
        shutdown_al();
    }

    Ref<AudioSource> AudioEngine::load_mp3(const std::filesystem::path& filename)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Loading .mp3: \"{}\"", filename.generic_string());

        Mp3FileInfo info(m_mp3Decoder, filename);
        AudioSourceFormat format = get_audio_source_format(info.get_channels());

        return make_ref<AudioSource>(format,
            info.get_buffer(), info.get_size(), info.get_sample_rate(), info.get_length());
    }

    Ref<AudioSource> AudioEngine::load_ogg(const std::filesystem::path& filename)
    {
        DRK_ASSERT_DEBUG(false, ".ogg is currently unimplemented");
        return {};

        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Loading .ogg: \"{}\"", filename.generic_string());
    }

    void AudioEngine::play_source(Ref<AudioSource> source)
    {
        DRK_PROFILE_FUNCTION();

        refresh();
        source->play();
        m_playingSources.push_back(source);
    }

    void AudioEngine::unpause_source(Ref<AudioSource> source)
    {
        DRK_PROFILE_FUNCTION();

        DRK_ASSERT_DEBUG(source->is_paused(), "Source is not paused");
        source->play();
    }

    void AudioEngine::unpause_all(void)
    {
        DRK_PROFILE_FUNCTION();

        for (auto& source : m_playingSources)
            if (source->is_paused())
                unpause_source(source);
    }

    void AudioEngine::pause_source(Ref<AudioSource> source)
    {
        DRK_PROFILE_FUNCTION();
        source->pause();
    }

    void AudioEngine::pause_all(void)
    {
        DRK_PROFILE_FUNCTION();

        for (auto& source : m_playingSources)
            if (source->is_playing())
                pause_source(source);
    }

    void AudioEngine::stop_source(Ref<AudioSource> source)
    {
        DRK_PROFILE_FUNCTION();

        if (!source->is_stopped())
            source->stop();

        auto it = std::ranges::find(m_playingSources, source);
        DRK_ASSERT_DEBUG(it != m_playingSources.end(), "Source is not playing");

        m_playingSources.erase(it);
    }

    void AudioEngine::stop_all(void)
    {
        DRK_PROFILE_FUNCTION();

        for (auto& source : m_playingSources)
            stop_source(source);
    }

    void AudioEngine::set_gain(float gain)
    {
        alListenerf(AL_GAIN, gain);
    }

    void AudioEngine::refresh(void)
    {
        DRK_PROFILE_FUNCTION();

        std::vector<Ref<AudioSource>> stoppedSources;
        for (const auto& source : m_playingSources)
            if (source->is_stopped())
                stoppedSources.push_back(source);

        for (auto& source : stoppedSources)
            stop_source(source);
    }

    ////////////////////////
    //       Volume       //
    ////////////////////////

    Volume::Volume(float vol)
      : m_volume(vol),
        m_muted(false)
    { }

    void Volume::set(float vol)
    {
        m_volume = vol;
        m_muted = false;
    }

    float Volume::get(void) const
    {
        if (m_muted)
            return 0.0f;
        else
            return m_volume;
    }

    void Volume::mute(void)
    {
        m_muted = true;
    }

    void Volume::unmute(void)
    {
        m_muted = false;
    }

    bool Volume::muted(void) const
    {
        return m_muted;
    }

    ///////////////////////
    //       Audio       //
    ///////////////////////

    AudioEngine* Audio::s_engine = nullptr;
    Volume Audio::s_volume = 0.0f;

    void Audio::init(float volume)
    {
        DRK_ASSERT_DEBUG(!s_engine, "AudioEngine is already initialized");

        s_engine = new AudioEngine;
        set_volume(volume);
    }

    void Audio::shutdown(void)
    {
        DRK_ASSERT_DEBUG(s_engine, "AudioEngine is already shutdown");
        delete s_engine;
    }

    bool Audio::running(void)
    {
        return static_cast<bool>(s_engine);
    }

    AudioEngine& Audio::get_engine(void)
    {
        DRK_ASSERT_DEBUG(s_engine, "AudioEngine is not running");
        return *s_engine;
    }

    Ref<AudioSource> Audio::load_file(const std::filesystem::path& filename)
    {
        DRK_PROFILE_FUNCTION();
        DRK_ASSERT_DEBUG_NO_MSG(running());

        DRK_ASSERT_CORE(std::filesystem::is_regular_file(filename),
            "Audio file \"{}\" does not exist", filename.generic_string());

        DRK_LOG_CORE_INFO("Loading Audio file: \"{}\"", filename.generic_string());

        Ref<AudioSource> source;
        auto extension = filename.extension();
        switch (find_audio_file_format(extension.string()))
        {
            case AudioFileFormat::Mp3 : source = get_engine().load_mp3(filename); break;
            case AudioFileFormat::Ogg : source = get_engine().load_ogg(filename); break;
            default:
                DRK_ASSERT_DEBUG(false, "File format {} not supported", extension.string());
                return {};
        }
        return source;
    }

    void Audio::play(Ref<AudioSource> source)
    {
        get_engine().play_source(source);
    }

    void Audio::play(void)
    {
        get_engine().unpause_all();
    }

    void Audio::pause(Ref<AudioSource> source)
    {
        get_engine().pause_source(source);
    }

    void Audio::pause(void)
    {
        get_engine().pause_all();
    }

    void Audio::stop(Ref<AudioSource> source)
    {
        get_engine().stop_source(source);
    }

    void Audio::stop(void)
    {
        get_engine().stop_all();
    }

    void Audio::set_volume(float vol)
    {
        s_volume.set(vol);
    }

    float Audio::get_volume(void)
    {
        return s_volume.get();
    }

    void Audio::mute(void)
    {
        s_volume.mute();
    }

    void Audio::unmute(void)
    {
        s_volume.unmute();
    }

    void Audio::toggle_mute(void)
    {
        if (s_volume.muted())
            s_volume.unmute();
        else
            s_volume.mute();
    }

    bool Audio::is_muted(void)
    {
        return s_volume.muted();
    }
}
