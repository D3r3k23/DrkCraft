#include "Util.hpp"

#include <AL/al.h>

namespace DrkCraft
{
    AudioFileFormat find_audio_file_format(const fs::path& filename)
    {
        return ext_to_audio_file_format(filename.extension().string());
    }

    AudioFileFormat ext_to_audio_file_format(std::string_view extension)
    {
        if (extension == ".mp3") return AudioFileFormat::Mp3;
        if (extension == ".ogg") return AudioFileFormat::Ogg;
        return AudioFileFormat::None;
    }

    AudioSourceFormat get_audio_source_format(uint channels)
    {
        switch (channels)
        {
            case 1 : return AudioSourceFormat::Mono16;
            case 2 : return AudioSourceFormat::Stereo16;
            default: return AudioSourceFormat::None;
        }
    }

    uint to_al_source_format(AudioSourceFormat format)
    {
        switch (format)
        {
            case AudioSourceFormat::Mono8    :    return AL_FORMAT_MONO8;
            case AudioSourceFormat::Mono16   :   return AL_FORMAT_MONO16;
            case AudioSourceFormat::Stereo8  :  return AL_FORMAT_STEREO8;
            case AudioSourceFormat::Stereo16 : return AL_FORMAT_STEREO16;
            default:
                DRK_ASSERT_DEBUG_FALSE("Unknown OpenAL format");
                return 0;
        }
    }
}
