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

    AudioSourceFormat get_audio_source_format(uint channels, uint bitDepth)
    {
        using enum AudioSourceFormat;
        if (channels == 1)
        {
            if (bitDepth == 8)  return Mono8;
            if (bitDepth == 16) return Mono16;
        }
        if (channels == 2)
        {
            if (bitDepth == 8)  return Stereo8;
            if (bitDepth == 16) return Stereo16;
        }
        return AudioSourceFormat::None;
    }

    uint to_al_source_format(AudioSourceFormat format)
    {
        using enum AudioSourceFormat;
        switch (format)
        {
            case Mono8    :    return AL_FORMAT_MONO8;
            case Mono16   :   return AL_FORMAT_MONO16;
            case Stereo8  :  return AL_FORMAT_STEREO8;
            case Stereo16 : return AL_FORMAT_STEREO16;
            default:
                DRK_ASSERT_DEBUG(false, "Unknown OpenAL format");
                return 0;
        }
    }
}
