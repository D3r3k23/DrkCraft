#include "Mp3Decoder.hpp"

#include "Core/Debug/Profiler.hpp"

namespace DrkCraft
{
    /////////////////////////
    //       Mp3Data       //
    /////////////////////////

    Mp3Data::Mp3Data(const mp3dec_file_info_t& info)
    {
        size = info.samples * sizeof(mp3d_sample_t);
        buffer = static_cast<int16*>(info.buffer);
        channels = static_cast<uint>(info.channels);
        sampleRate = static_cast<uint>(info.hz);
        bitRate  = static_cast<uint>(info.avg_bitrate_kbps);
        format = get_audio_source_format(channels);
    }

    Mp3Data::~Mp3Data(void)
    {
        free(buffer);
    }

    ////////////////////////////
    //       Mp3Decoder       //
    ////////////////////////////

    Mp3Decoder::Mp3Decoder(void)
    {
        DRK_LOG_CORE_TRACE("Initializing Mp3Decoder");
        mp3dec_init(&m_decoder);
    }

    Ptr<const Mp3Data> Mp3Decoder::load_file(const fs::path& path)
    {
        DRK_PROFILE_FUNCTION();
        DRK_ASSERT_DEBUG_NO_MSG(find_audio_file_format(path) == AudioFileFormat::Mp3);

        mp3dec_file_info_t info;
        std::lock_guard lock(m_mutex);
        {
            DRK_PROFILE_SCOPE("Decode mp3");
            int result = mp3dec_load(&m_decoder, path.string().c_str(), &info, nullptr, nullptr);
            DRK_ASSERT_CORE(result == 0, "mp3dec_load failed");
        }
        return make_ptr<const Mp3Data>(info);
    }
}
