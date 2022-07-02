#ifndef DRK_AUDIO_AUDIO_HPP
#define DRK_AUDIO_AUDIO_HPP

#include "Core/Base.hpp"
#include "Audio/AudioSource.hpp"

#include "Lib/fs.hpp"
#include "Lib/string_view.hpp"

namespace DrkCraft
{
    class Volume
    {
    public:
        Volume(float vol=0.0f);

        void set(float vol);
        float get(void) const;

        void mute(void);
        void unmute(void);
        bool muted(void) const;

    private:
        float m_volume;
        bool m_muted;
    };

    // Public interface to static AudioEngine instance
    class Audio
    {
    public:
        static void init(float volume);
        static void shutdown(void);

        static bool running(void);

        static Ref<AudioSource> load_file(const fs::path& filename);

        static void play(Ref<AudioSource> source);
        static void play(void);

        static void pause(Ref<AudioSource> source);
        static void pause(void);

        static void stop(Ref<AudioSource> source);
        static void stop(void);

        static void set_volume(float vol);
        static float get_volume(void);

        static void mute(void);
        static void unmute(void);
        static void toggle_mute(void);
        static bool is_muted(void);

        static string_view get_device_name(void);

    private:
        static Volume s_volume;
    };
}

#endif // DRK_AUDIO_AUDIO_HPP
