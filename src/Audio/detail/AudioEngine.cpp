#include "AudioEngine.hpp"

#include "Audio/detail/Util.hpp"
#include "Core/Debug/Profiler.hpp"

#include <AL/al.h>

#include <algorithm>

namespace DrkCraft
{
    static constexpr float MASTER_LISTENER_GAIN = 0.25f; // -> 0.5 after volume ctrl added

    AudioEngine::AudioEngine(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Initializing AudioEngine");

        DRK_LOG_CORE_TRACE("Initializing OpenAL device");
        m_device = make_ptr<AlDevice>();
        DRK_LOG_CORE_INFO("Audio device: {}", m_device->get_name());

        DRK_LOG_CORE_TRACE("Initializing OpenAL context");
        m_context = make_ptr<AlContext>(*m_device);
        m_context->make_current();

        ALfloat listenerPos[] = { 0.0f, 0.0f, 0.0f };
        ALfloat listenerVel[] = { 0.0f, 0.0f, 0.0f };
        ALfloat listenerOri[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f }; // Forward

        alListenerfv(AL_POSITION,    listenerPos);
        alListenerfv(AL_VELOCITY,    listenerVel);
        alListenerfv(AL_ORIENTATION, listenerOri);

        set_gain(MASTER_LISTENER_GAIN);
    }

    AudioEngine::~AudioEngine(void)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("Stopping all playing audio sources");
        stop_all();

        DRK_LOG_CORE_TRACE("Shutting down OpenAL");
        AlContext::clear_current();
    }

    Ref<AudioSource> AudioEngine::load_mp3(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("Loading .mp3: \"{}\"", filename.generic_string());
        DRK_ASSERT_DEBUG_NO_MSG(find_audio_file_format(filename) == AudioFileFormat::Mp3);

        auto data = m_mp3Decoder.decode(filename);
        return make_ref<AudioSource>(*data);
    }

    Ref<AudioSource> AudioEngine::load_ogg(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("Loading .ogg: \"{}\"", filename.generic_string());
        DRK_ASSERT_DEBUG_NO_MSG(find_audio_file_format(filename) == AudioFileFormat::Ogg);

        auto data = OggDecoder::decode(filename);
        return make_ref<AudioSource>(*data);
    }

    void AudioEngine::play_source(const Ref<AudioSource>& source)
    {
        DRK_PROFILE_FUNCTION();

        refresh();
        source->play();
        m_playingSources.push_back(source);
    }

    void AudioEngine::unpause_source(const Ref<AudioSource>& source)
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

    void AudioEngine::pause_source(const Ref<AudioSource>& source)
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

    void AudioEngine::stop_source(const Ref<AudioSource>& source)
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

    const AlDevice& AudioEngine::get_device(void) const
    {
        return *m_device;
    }

    const AlContext& AudioEngine::get_context(void) const
    {
        return *m_context;
    }
}
