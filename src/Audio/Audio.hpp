#ifndef DRK_AUDIO_HPP
#define DRK_AUDIO_HPP

#include "Core/Base.hpp"
#include "AudioSource.hpp"

#include <minimp3/minimp3.h>
#include <glm/vec3.hpp>

#include <deque>
#include <filesystem>
#include <concepts>

namespace DrkCraft
{
    class Mp3Decoder
    {
    public:
        Mp3Decoder(void);
        ~Mp3Decoder(void) = default;

        mp3dec_t& get(void);

    private:
        mp3dec_t m_decoder;
    };

    class AudioEngine
    {
        friend class Audio;

    private:
        AudioEngine(void);
        ~AudioEngine(void);

        Ref<AudioSource> load_mp3(const std::filesystem::path& filename);
        Ref<AudioSource> load_ogg(const std::filesystem::path& filename);

        void play_source(Ref<AudioSource> source);
        void unpause_source(Ref<AudioSource> source);
        void unpause_all(void);

        void pause_source(Ref<AudioSource> source);
        void pause_all(void);

        void stop_source(Ref<AudioSource> source);
        void stop_all(void);

        void set_gain(float gain);

        void refresh(void);

    private:
        std::deque<Ref<AudioSource>> m_playingSources;
        Mp3Decoder m_mp3Decoder;
    };

    // Public interface to static AudioEngine singleton
    class Audio
    {
    public:
        static void init(void);
        static void shutdown(void);

        static bool running(void);
        static AudioEngine& get_engine(void);

        static Ref<AudioSource> load_file(const std::filesystem::path& filename);

        static void play(Ref<AudioSource> source);
        static void play(void);

        static void pause(Ref<AudioSource> source);
        static void pause(void);

        static void stop(Ref<AudioSource> source);
        static void stop(void);

    private:
        static AudioEngine* s_engine;
    };
}

#endif // DRK_AUDIO_HPP
