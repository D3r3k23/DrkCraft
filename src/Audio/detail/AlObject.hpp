#ifndef DRK_AUDIO_DETAIL_AL_OBJECT_HPP
#define DRK_AUDIO_DETAIL_AL_OBJECT_HPP

#include "Core/Base.hpp"

namespace DrkCraft
{
    using AlObjectID = uint;

    class AlObject
    {
    protected:
        AlObject(void) = default;
        virtual ~AlObject(void) = default;

    public:
        AlObject(const AlObject&) = delete;
        AlObject& operator=(const AlObject&) = delete;

        AlObjectID get_id(void) const
        {
            return m_id;
        }

    protected:
        AlObjectID m_id;
    };
}

#endif // DRK_AUDIO_DETAIL_AL_OBJECT_HPP
