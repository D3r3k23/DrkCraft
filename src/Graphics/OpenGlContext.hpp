#ifndef DRK_GRAPHICS_OPEN_GL_CONTEXT_HPP
#define DRK_GRAPHICS_OPEN_GL_CONTEXT_HPP

#include "Core/Base.hpp"

#include <string>
#include <string_view>

namespace DrkCraft
{
    class Window;

    class OpenGlContext
    {
    public:
        OpenGlContext(Window& window);
        ~OpenGlContext(void);

        void swap_buffers(void);
        void make_current(void);
        static void clear_current(void);

        std::string_view get_gl_version(void) const;
        std::string_view get_renderer_info(void) const;

    private:
        void get_context_info(void);

    private:
        Window& m_window;

        std::string m_glVersion;
        std::string m_rendererHardware;
    };
}

#endif // DRK_GRAPHICS_OPEN_GL_CONTEXT_HPP
