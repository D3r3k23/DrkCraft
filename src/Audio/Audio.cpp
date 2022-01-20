#include "Audio.hpp"

#include "AlTools.hpp"
#include "Core/Profiler.hpp"

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

    Ref<AudioSource> Audio::load_file(const std::filesystem::path& path)
    {
        DRK_PROFILE_FUNCTION();
        DRK_ASSERT_DEBUG_NO_MSG(running());

        if (std::filesystem::is_regular_file(path))
        {
            DRK_LOG_CORE_TRACE("Loading Audio file: \"{}\"", path.generic_string());

            Ref<AudioSource> source;
            auto extension = path.extension().string();
            switch (ext_to_audio_file_format(extension))
            {
                case AudioFileFormat::Mp3 : source = get_engine().load_mp3(path); break;
                case AudioFileFormat::Ogg : source = get_engine().load_ogg(path); break;
                default:
                    DRK_ASSERT_DEBUG(false, "File format {} not supported", extension);
                    return {};
            }
            return source;
        }
        else
        {
            DRK_LOG_CORE_ERROR("Audio file \"{}\" not found", path.generic_string());
            return {};
        }
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
