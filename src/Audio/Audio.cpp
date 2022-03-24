#include "Audio.hpp"

#include "Audio/detail/AudioEngine.hpp"
#include "Audio/detail/AlTools.hpp"
#include "Util/File.hpp"
#include "Core/Debug/Profiler.hpp"

namespace DrkCraft
{
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

    static std::optional<AudioEngine> s_audioEngine;

    static AudioEngine& get_audio_engine(void)
    {
        DRK_ASSERT_DEBUG(s_audioEngine, "AudioEngine is not running");
        return *s_audioEngine;
    }

    Volume Audio::s_volume;

    void Audio::init(float volume)
    {
        DRK_ASSERT_DEBUG(!s_audioEngine, "AudioEngine is already initialized");

        s_audioEngine.emplace();
        set_volume(volume);
    }

    void Audio::shutdown(void)
    {
        DRK_ASSERT_DEBUG(s_audioEngine, "AudioEngine is already shutdown");
        s_audioEngine.reset();
    }

    bool Audio::running(void)
    {
        return static_cast<bool>(s_audioEngine);
    }

    Ref<AudioSource> Audio::load_file(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();
        DRK_ASSERT_DEBUG_NO_MSG(running());

        if (is_file(filename))
        {
            DRK_LOG_CORE_TRACE("Loading Audio file: \"{}\"", filename.generic_string());
            return get_audio_engine().load_source(filename);
        }
        else
        {
            DRK_LOG_CORE_ERROR("Audio file \"{}\" not found", filename.generic_string());
            return {};
        }
    }

    void Audio::play(Ref<AudioSource> source)
    {
        get_audio_engine().play_source(source);
    }

    void Audio::play(void)
    {
        get_audio_engine().unpause_all();
    }

    void Audio::pause(Ref<AudioSource> source)
    {
        get_audio_engine().pause_source(source);
    }

    void Audio::pause(void)
    {
        get_audio_engine().pause_all();
    }

    void Audio::stop(Ref<AudioSource> source)
    {
        get_audio_engine().stop_source(source);
    }

    void Audio::stop(void)
    {
        get_audio_engine().stop_all();
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

    std::string_view Audio::get_device_name(void)
    {
        return get_audio_engine().get_device().get_name();
    }
}
