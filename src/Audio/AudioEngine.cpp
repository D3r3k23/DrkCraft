#include "Audio.hpp"

#include "AlTools.hpp"
#include "Core/Profiler.hpp"

#include <AL/al.h>
#include <AL/alc.h>

#include <string_view>
#include <algorithm>
#include <cstdlib>

namespace DrkCraft
{
    static const float MASTER_LISTENER_GAIN = 0.25f; // -> 0.5 after volume ctrl added

    AudioFileFormat find_audio_file_format(const std::filesystem::path& filename)
    {
        return ext_to_audio_file_format(filename.extension().string());
    }

    AudioFileFormat ext_to_audio_file_format(std::string_view extension)
    {
        if (extension == ".mp3") return AudioFileFormat::Mp3;
        if (extension == ".ogg") return AudioFileFormat::Ogg;
        return AudioFileFormat::None;
    }

    /////////////////////////
    //       Mp3Data       //
    /////////////////////////

    Mp3Data::Mp3Data(const mp3dec_file_info_t& info)
    {
        size  = info.samples * sizeof(mp3d_sample_t);
        buffer  = static_cast<int16*>(info.buffer);
        channels  = static_cast<uint>(info.channels);
        sampleRate  = static_cast<uint>(info.hz);
        bitRate   = static_cast<uint>(info.avg_bitrate_kbps);
        format  = get_audio_source_format(channels);
    }

    Mp3Data::~Mp3Data(void)
    {
        free(buffer);
    }

    ////////////////////////////
    //       Mp3Decoder       //
    ////////////////////////////

    Mp3Decoder::Mp3Decoder(void)
    {
        DRK_LOG_CORE_TRACE("Initializing Mp3Decoder");
        mp3dec_init(&m_decoder);
    }

    Ptr<const Mp3Data> Mp3Decoder::load_file(const std::filesystem::path& path)
    {
        DRK_PROFILE_FUNCTION();
        DRK_ASSERT_DEBUG_NO_MSG(find_audio_file_format(path) == AudioFileFormat::Mp3);

        mp3dec_file_info_t info;
        std::lock_guard lock(m_mutex);
        {
            DRK_PROFILE_SCOPE("Decode mp3");
            int result = mp3dec_load(&m_decoder, path.string().c_str(), &info, nullptr, nullptr);
            DRK_ASSERT_CORE(result == 0, "mp3dec_load failed");
        }
        return make_ptr<const Mp3Data>(info);
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

    Ref<AudioSource> AudioEngine::load_mp3(const std::filesystem::path& path)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("Loading .mp3: \"{}\"", path.generic_string());
        DRK_ASSERT_DEBUG_NO_MSG(find_audio_file_format(path) == AudioFileFormat::Mp3);

        auto data = m_mp3Decoder.load_file(path);

        return make_ref<AudioSource>(data->format, data->buffer, data->size,
            data->sampleRate, data->bitRate);
    }

    Ref<AudioSource> AudioEngine::load_ogg(const std::filesystem::path& path)
    {
    #if 0
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("Loading .ogg: \"{}\"", path.generic_string());
        DRK_ASSERT_DEBUG_NO_MSG(find_audio_file_format(path) == AudioFileFormat::Ogg);
    #endif
        DRK_ASSERT_DEBUG(false, ".ogg is currently unimplemented");
        return {};
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
}
