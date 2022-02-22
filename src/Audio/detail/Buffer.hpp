#ifndef DRK_AUDIO_DETAIL_BUFFER_HPP
#define DRK_AUDIO_DETAIL_BUFFER_HPP

#include "Core/Base.hpp"
#include "Audio/detail/AlObject.hpp"
#include "Audio/detail/Util.hpp"

namespace DrkCraft
{
    class AlBuffer : public AlObject
    {
    public:
        AlBuffer(AudioSourceFormat format, const void* data, uint size, uint sampleRate);
        virtual ~AlBuffer(void);

    private:
        AudioSourceFormat m_format;
    };
}

#endif // DRK_AUDIO_DETAIL_BUFFER_HPP
