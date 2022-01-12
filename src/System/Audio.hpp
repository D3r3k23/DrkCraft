#ifndef DRK_AUDIO_HPP
#define DRK_AUDIO_HPP

#include "Core/Base.hpp"

#include <filesystem>
#include <concepts>

namespace DrkCraft
{
    class AudioSource
    {
    public:
        AudioSource(void) = default;
        virtual ~AudioSource(void) = default;

    protected:

    };

    enum class AudioSourceFormat
    {
        None = 0,
        Mp3,
        Ogg
    };

    class AudioSourceMp3 : public AudioSource
    {
    public:
        AudioSourceMp3(const std::filesystem::path& filename);
        ~AudioSourceMp3(void) = default;
    };

    class AudioSourceOgg : public AudioSource
    {
    public:
        AudioSourceOgg(const std::filesystem::path& filename);
        ~AudioSourceOgg(void) = default;
    };

    namespace Audio
    {
        AudioSourceFormat find_format(const std::filesystem::path& filename);
        Ptr<AudioSource> load_file(const std::filesystem::path& filename);
        void play(const AudioSource& source);
    }
}

#endif // DRK_AUDIO_HPP
