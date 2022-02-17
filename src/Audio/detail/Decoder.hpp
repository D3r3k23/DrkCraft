#ifndef DRK_AUDIO_DETAIL_DECODER_HPP
#define DRK_AUDIO_DETAIL_DECODER_HPP

#include "Core/Base.hpp"
#include "Audio/AudioSource.hpp"
#include "Audio/detail/Util.hpp"

#include "lib/fs.hpp"

#include <vector>

namespace DrkCraft
{
    class Mp3Decoder
    {
    public:
        Mp3Decoder(void);
        ~Mp3Decoder(void) = default;

        Ptr<const AudioSourceData<int16>> decode(const fs::path& filename) const;
    };

    class OggDecoder
    {
    public:
        static Ptr<const AudioSourceData<Byte>> decode(const fs::path& filename);
    };
}

#endif // DRK_AUDIO_DETAIL_DECODER_HPP
