#include "Audio.hpp"

namespace DrkCraft
{
    AudioSourceMp3::AudioSourceMp3(const std::filesystem::path& filename)
    {

    }

    AudioSourceOgg::AudioSourceOgg(const std::filesystem::path& filename)
    {

    }

    namespace Audio
    {
        AudioSourceFormat find_format(const std::filesystem::path& filename)
        {
            auto extension = filename.extension();

            if (extension == ".mp3")
                return AudioSourceFormat::Mp3;
            if (extension == ".ogg")
                return AudioSourceFormat::Ogg;

            return AudioSourceFormat::None;
        }

        Ptr<AudioSource> load_file(const std::filesystem::path& filename)
        {
            switch (find_format(filename))
            {
                case AudioSourceFormat::Mp3 : return make_ptr<AudioSourceMp3>(filename);
                case AudioSourceFormat::Ogg : return make_ptr<AudioSourceOgg>(filename);
                default: DRK_ASSERT_DEBUG(false, "Audio file {} not supported", filename.string());
            }
        }

        void play(const AudioSource& source)
        {

        }
    }
}
