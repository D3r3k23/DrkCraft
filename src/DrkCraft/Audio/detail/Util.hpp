#ifndef DRK_AUDIO_DETAIL_UTIL_HPP
#define DRK_AUDIO_DETAIL_UTIL_HPP

#include "Core/Base.hpp"

#include "Lib/fs.hpp"
#include "Lib/fs.hpp"

namespace DrkCraft
{
    enum class AudioFileFormat
    {
        None = 0,
        Mp3,
        Ogg
    };

    AudioFileFormat find_audio_file_format(const fs::path& filename);
    AudioFileFormat ext_to_audio_file_format(string_view extension);

    enum class AudioSourceFormat
    {
        None = 0,
        Mono8,
        Mono16,
        Stereo8,
        Stereo16
    };

    AudioSourceFormat get_audio_source_format(uint channels);
    uint to_al_source_format(AudioSourceFormat format);
}

#endif // DRK_AUDIO_DETAIL_UTIL_HPP
