#ifndef DRK_AL_OBJECT_HPP
#define DRK_AL_OBJECT_HPP

#include "Core/Base.hpp"

namespace DrkCraft
{
    using AlObjectID = uint;

    class AlObject
    {
    public:
        AlObject(void) = default;
        virtual ~AlObject(void) = default;

        AlObjectID get_id(void) const;

    protected:
        AlObjectID m_id;
    };
}

#endif // DRK_AL_OBJECT_HPP
