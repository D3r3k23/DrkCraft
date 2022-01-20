#ifndef DRK_AUDIO_ENGINE_HPP
#define DRK_AUDIO_ENGINE_HPP

#include "Core/Base.hpp"
#include "AudioSource.hpp"

#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>

#include <deque>
#include <filesystem>
#include <concepts>
#include <mutex>

namespace DrkCraft
{
    enum class AudioFileFormat
    {
        None = 0,
        Mp3,
        Ogg
    };

    AudioFileFormat find_audio_file_format(std::string_view extension);
    AudioFileFormat ext_to_audio_file_format(std::string_view extension);

    struct Mp3Data
    {
        using SampleBuffer_t = mp3d_sample_t;
        // Ensure that buffer can be stored as int16*
        static_assert(std::signed_integral<SampleBuffer_t>);
        static_assert(sizeof(int16) == sizeof(SampleBuffer_t));

        Mp3Data(const mp3dec_file_info_t& info);
        ~Mp3Data(void);

        int16* buffer;
        uint   size;
        uint   channels;
        uint   bitRate; // kB/s
        uint   sampleRate;
        float  length; // Seconds
        AudioSourceFormat format;
    };

    class Mp3Decoder
    {
    public:
        Mp3Decoder(void);
        ~Mp3Decoder(void) = default;

        Ptr<const Mp3Data> load_file(const std::filesystem::path& path);

    private:
        mp3dec_t m_decoder;
        std::mutex m_mutex;
    };

    // Loading is thread safe, playing/control is not
    class AudioEngine
    {
        friend class Audio;

    private:
        AudioEngine(void);
        ~AudioEngine(void);

        Ref<AudioSource> load_mp3(const std::filesystem::path& path);
        Ref<AudioSource> load_ogg(const std::filesystem::path& path);

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
}

#endif // DRK_AUDIO_ENGINE_HPP
