#ifndef DRK_GRAPHICS_OPEN_GL_CONTEXT_HPP
#define DRK_GRAPHICS_OPEN_GL_CONTEXT_HPP

#include "Core/Base.hpp"
#include "System/Window.hpp"

#include <GLFW/glfw3.h>

namespace DrkCraft
{
    class OpenGlContext
    {
    public:
        OpenGlContext(Window& window);
        ~OpenGlContext(void);

        void swap_buffers(void);
        void make_current(void);
        static void clear_current(void);

    private:
        GLFWwindow* m_window;
    };
}

#endif // DRK_GRAPHICS_OPEN_GL_CONTEXT_HPP
