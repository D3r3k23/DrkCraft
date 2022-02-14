#include "Decoder.hpp"

#include "Util/ByteBuffer.hpp"
#include "Core/Debug/Profiler.hpp"

#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include <mutex>
#include <cstdio>
#include <cstdlib>

namespace DrkCraft
{
    /////////////////////////
    //       Mp3Data       //
    /////////////////////////

    // Ensure that buffer can be stored as int16*
    using SampleBuffer_t = mp3d_sample_t;
    static_assert(std::signed_integral<SampleBuffer_t>);
    static_assert(sizeof(int16) == sizeof(SampleBuffer_t));

    ////////////////////////////
    //       Mp3Decoder       //
    ////////////////////////////

    static mp3dec_t s_mp3Decoder;
    static std::mutex s_mp3DecoderMutex;

    Mp3Decoder::Mp3Decoder(void)
    {
        DRK_LOG_CORE_TRACE("Initializing Mp3Decoder");
        std::lock_guard lock(s_mp3DecoderMutex);
        mp3dec_init(&s_mp3Decoder);
    }

    Ptr<const AudioSourceData<int16>> Mp3Decoder::decode(const fs::path& filename) const
    {
        DRK_PROFILE_FUNCTION();
        DRK_ASSERT_DEBUG_NO_MSG(find_audio_file_format(filename) == AudioFileFormat::Mp3);
        std::lock_guard lock(s_mp3DecoderMutex);

        mp3dec_file_info_t info;
        {
            DRK_PROFILE_SCOPE("Decode mp3");
            int result = mp3dec_load(&s_mp3Decoder, filename.string().c_str(), &info, nullptr, nullptr);
            DRK_ASSERT_CORE(result == 0, "mp3dec_load failed");
        }
        auto data = new AudioSourceData<int16>(static_cast<int16*>(info.buffer), [](int16* buffer){ std::free(buffer); });

        data->size       = info.samples * sizeof(mp3d_sample_t);
        data->channels   = static_cast<uint>(info.channels);
        data->sampleRate = static_cast<uint>(info.hz);
        data->bitRate    = static_cast<uint>(info.avg_bitrate_kbps);
        data->bitDepth   = (data->bitRate * 1000 ) / (data->sampleRate * data->channels);
        data->format     = get_audio_source_format(data->channels, data->bitDepth);

        return Ptr<const AudioSourceData<int16>>(data);
    }

    ////////////////////////////
    //       OggDecoder       //
    ////////////////////////////

    namespace
    {
        class VorbisFile
        {
            using Info = vorbis_info;

        public:
            VorbisFile(const fs::path& filename)
            {
                DRK_PROFILE_FUNCTION();
                DRK_ASSERT_DEBUG_NO_MSG(find_audio_file_format(filename) == AudioFileFormat::Ogg);

                int callbacks = ov_fopen(filename.string().c_str(), &m_vorbisFile);
                DRK_ASSERT_CORE(callbacks != -1, "Could not open ogg stream");
            }

            ~VorbisFile(void)
            {
                DRK_PROFILE_FUNCTION();
                ov_clear(&m_vorbisFile);
            }

            const VorbisFile::Info& info(int section=-1)
                { return *ov_info(&m_vorbisFile, section); }

            uint sample_rate(int section=-1) { return info().rate; }

            uint samples(int section=-1)
                { return ov_pcm_total(&m_vorbisFile, section); }

        private:
            OggVorbis_File m_vorbisFile;
        };
    }

    Ptr<const AudioSourceData<uint8>> OggDecoder::decode(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        VorbisFile vorbisFile(filename);

        const auto& info = vorbisFile.info();
        const uint64 samples  = vorbisFile.samples();
        const uint sampleRate = info.rate;
        const uint  channels  = info.channels;
        const uint    size    = 2 * channels * samples;

        uint8* buffer = new uint8[size];

        // Fill buffer
        DRK_ASSERT_DEBUG(false, ".ogg is currently unimplemented");

        auto data = new AudioSourceData<uint8>(buffer, [](uint8* buffer){ delete[] buffer; });

        data->size       = samples * sizeof(uint8);
        data->channels   = channels;
        data->sampleRate = sampleRate;
        data->bitRate    = 0; // avgBitRate;
        data->bitDepth   = (data->bitRate * 1000 ) / (sampleRate * channels);
        data->format     = get_audio_source_format(data->channels, data->bitDepth);

        return Ptr<const AudioSourceData<uint8>>(data);
    }
}
