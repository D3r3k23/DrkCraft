#include "OpenGlObject.hpp"

#include "Core/Profiler.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
    GlObjectID OpenGlObject::get_id(void) const
    {
        return m_id;
    }
}
