#include "Buffer.hpp"

#include "Core/Debug/Profiler.hpp"

#include <AL/al.h>

namespace DrkCraft
{
    AlBuffer::AlBuffer(AudioSourceFormat format, void* data, uint size, uint sampleRate)
    {
        DRK_PROFILE_FUNCTION();

        alGenBuffers(1, &m_id);
        alBufferData(m_id, to_al_source_format(format), data, size, sampleRate);
    }

    AlBuffer::~AlBuffer(void)
    {
        DRK_PROFILE_FUNCTION();
        alDeleteBuffers(1, &m_id);
    }
}
