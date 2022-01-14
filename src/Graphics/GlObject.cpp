#include "GlObject.hpp"

#include "Core/Profiler.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
    GlObjectID GlObject::get_id(void) const
    {
        return m_id;
    }
}
