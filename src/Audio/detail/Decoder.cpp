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
    ////////////////////////////
    //       Mp3Decoder       //
    ////////////////////////////

    // Ensure that buffer can be stored as int16*
    using SampleBuffer_t = mp3d_sample_t;
    static_assert(std::signed_integral<SampleBuffer_t>);
    static_assert(sizeof(int16) == sizeof(SampleBuffer_t));

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
            DRK_PROFILE_SCOPE("mp3dec_load");
            int result = mp3dec_load(&s_mp3Decoder, filename.string().c_str(), &info, nullptr, nullptr);
            if (result != 0)
            {
                DRK_LOG_CORE_ERROR("Mp3Decoder failed to decode \"{}\"", filename.generic_string());
                return {};
            }
        }
        auto buffer = make_ptr<int16>(static_cast<int16*>(info.buffer), [](int16* buffer){ std::free(buffer); });
        auto data = new AudioSourceData<int16>();

        const auto samples = info.samples;

        data->buffer  = move(buffer);
        data->size     = samples * sizeof(int16);
        data->channels  = static_cast<uint>(info.channels);
        data->sampleRate = static_cast<uint>(info.hz);
        data->length    = static_cast<float>(samples) / static_cast<float>(data->sampleRate);
        data->format   = get_audio_source_format(data->channels);

        return Ptr<const AudioSourceData<int16>>(data);
    }

    ////////////////////////////
    //       OggDecoder       //
    ////////////////////////////

    namespace
    {
        class VorbisFile
        {
        public:
            VorbisFile(const fs::path& filename)
            {
                DRK_PROFILE_FUNCTION();
                DRK_ASSERT_DEBUG_NO_MSG(find_audio_file_format(filename) == AudioFileFormat::Ogg);
                {
                    DRK_PROFILE_SCOPE("ov_open");
                    int callbacks = ov_fopen(filename.string().c_str(), &m_vorbisFile);
                    DRK_ASSERT_CORE(callbacks != -1, "Could not open ogg stream");
                }
                const auto info = ov_info(&m_vorbisFile, -1);
                m_samples      = static_cast<uint64>(ov_pcm_total(&m_vorbisFile, -1));
                m_sampleRate  = static_cast<uint>(info->rate);
                m_channels   = static_cast<uint>(info->channels);
                m_size      = 2 * m_channels * m_samples;
            }

            ~VorbisFile(void)
            {
                DRK_PROFILE_FUNCTION();
                ov_clear(&m_vorbisFile);
            }

            uint8* read(void)
            {
                DRK_PROFILE_FUNCTION();

                static constexpr uint READ_SIZE = 4096;

                Byte* buffer;
                {
                    DRK_PROFILE_SCOPE("Allocate buffer");
                    buffer = new Byte[m_size];
                }
                Byte* bufPtr = buffer;

                bool eof = false;
                bool error = false;
                while (!eof && !error)
                {
                    int section;
                    long n;
                    {
                        DRK_PROFILE_SCOPE("ov_read");
                        n = ov_read(&m_vorbisFile, (char*)(bufPtr), READ_SIZE, 0, 1, 0, &section);
                    }
                    if (n == 0)
                        eof = true;
                    else if (n < 0)
                        error = true;
                    else
                        bufPtr += n;
                }
                if (error)
                {
                    DRK_ASSERT_DEBUG(false, "Error while reading Vorbis file");
                    delete[] buffer;
                    return nullptr;
                }
                else
                    return buffer;
            }

        uint64 sample_rate(void) const { return m_sampleRate; }
        uint64 samples(void)   const { return m_samples; }
        uint64 channels(void) const { return m_channels; }
        uint64 size(void)   const { return m_size; }

        private:
            OggVorbis_File m_vorbisFile;
            uint m_sampleRate;
            uint64 m_samples;
            uint m_channels;
            uint m_size;
        };
    }

    Ptr<const AudioSourceData<Byte>> OggDecoder::decode(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        VorbisFile vorbisFile(filename);
        if (Byte* readBuffer = vorbisFile.read(); readBuffer)
        {
            auto buffer = make_ptr<Byte>(readBuffer, [](Byte* buffer){ delete[] buffer; });
            auto data = new AudioSourceData<uint8>();

            data->buffer  = move(buffer);
            data->size     = vorbisFile.size();
            data->channels  = vorbisFile.channels();
            data->sampleRate = vorbisFile.sample_rate();
            data->length    = static_cast<float>(vorbisFile.samples()) / static_cast<float>(data->sampleRate);
            data->format   = get_audio_source_format(data->channels);

            return Ptr<const AudioSourceData<Byte>>(data);
        }
        else
        {
            DRK_LOG_CORE_ERROR("OggDecoder failed to decode \"{}\"", filename.generic_string());
            return {};
        }
    }
}
