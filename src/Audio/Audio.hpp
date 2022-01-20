#ifndef DRK_AUDIO_HPP
#define DRK_AUDIO_HPP

#include "Core/Base.hpp"
#include "AudioEngine.hpp"
#include "AudioSource.hpp"

#include <filesystem>

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

    // Public interface to static AudioEngine singleton
    class Audio
    {
    public:
        static void init(float volume);
        static void shutdown(void);

        static bool running(void);
        static AudioEngine& get_engine(void);

        static Ref<AudioSource> load_file(const std::filesystem::path& path);

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

    private:
        static AudioEngine* s_engine;
        static Volume s_volume;
    };
}

#endif // DRK_AUDIO_HPP
