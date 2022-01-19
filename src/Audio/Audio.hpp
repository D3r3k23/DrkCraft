#ifndef DRK_AUDIO_HPP
#define DRK_AUDIO_HPP

#include "Core/Base.hpp"
#include "AudioSource.hpp"

#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>
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

    class Mp3FileInfo
    {
    public:
        Mp3FileInfo(Mp3Decoder& decoder, const std::filesystem::path& filename);
        ~Mp3FileInfo(void);

        int16* get_buffer(void);
        uint get_size(void) const;
        float get_length(void) const;
        uint get_channels(void) const;
        uint get_sample_rate(void) const;

    private:
        mp3d_sample_t* m_buffer;
        uint m_size;
        float m_length; // Seconds
        uint m_channels;
        uint m_sampleRate;
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

        static Ref<AudioSource> load_file(const std::filesystem::path& filename);

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
