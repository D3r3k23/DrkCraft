#ifndef DRK_GRAPHICS_LIGHT_SOURCE_CPP
#define DRK_GRAPHICS_LIGHT_SOURCE_CPP

#include "Core/Base.hpp"

#include "Lib/glm/vec.hpp"

namespace DrkCraft
{
    struct LightSource
    {
        vec3 position;
        vec3 direction;
        vec4 color;
    };
}

#endif // DRK_GRAPHICS_LIGHT_SOURCE_CPP
