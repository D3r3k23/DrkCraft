#ifndef DRK_GRAPHICS_DETAIL_GL_TOOLS_HPP
#define DRK_GRAPHICS_DETAIL_GL_TOOLS_HPP

#include "Core/Base.hpp"

namespace DrkCraft
{
    void load_opengl(void);

    void DRK_APIENTRY gl_message_handler(uint source, uint type, uint id,
        uint severity, int length, const char* msg, const void* userParam);
}

#endif // DRK_GRAPHICS_DETAIL_GL_TOOLS_HPP
