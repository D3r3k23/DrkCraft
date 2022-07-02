#ifndef DRK_AUDIO_DETAIL_DECODER_HPP
#define DRK_AUDIO_DETAIL_DECODER_HPP

#include "Core/Base.hpp"
#include "Audio/AudioSource.hpp"
#include "Audio/detail/Util.hpp"

#include "Lib/fs.hpp"

#include <vector>

namespace DrkCraft
{
    class Mp3Decoder
    {
    public:
        Mp3Decoder(void);
        ~Mp3Decoder(void) = default;

        Ref<AudioSource> decode(const fs::path& filename) const;
    };

    class OggDecoder
    {
    public:
        static Ref<AudioSource> decode(const fs::path& filename);
    };
}

#endif // DRK_AUDIO_DETAIL_DECODER_HPP
