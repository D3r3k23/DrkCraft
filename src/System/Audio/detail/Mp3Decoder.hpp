#ifndef DRK_SYSTEM_AUDIO_DETAIL_MP3_DECODER_HPP
#define DRK_SYSTEM_AUDIO_DETAIL_MP3_DECODER_HPP

#include "Core/Base.hpp"
#include "System/Audio/detail/Util.hpp"

#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>

#include <concepts>
#include <mutex>

namespace DrkCraft
{
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

        Ptr<const Mp3Data> load_file(const fs::path& path);

    private:
        mp3dec_t m_decoder;
        std::mutex m_mutex;
    };
}

#endif // DRK_SYSTEM_AUDIO_DETAIL_MP3_DECODER_HPP
