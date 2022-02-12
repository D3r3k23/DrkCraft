#ifndef DRK_SYSTEM_AUDIO_DETAIL_BUFFER_HPP
#define DRK_SYSTEM_AUDIO_DETAIL_BUFFER_HPP

#include "Core/Base.hpp"
#include "System/Audio/detail/AlObject.hpp"
#include "System/Audio/detail/Util.hpp"

namespace DrkCraft
{
    class AlBuffer : public AlObject
    {
    public:
        AlBuffer(AudioSourceFormat format, int16* data, uint size, uint sampleRate);
        virtual ~AlBuffer(void);

    private:
        AudioSourceFormat m_format;
    };
}

#endif // DRK_SYSTEM_AUDIO_DETAIL_BUFFER_HPP
