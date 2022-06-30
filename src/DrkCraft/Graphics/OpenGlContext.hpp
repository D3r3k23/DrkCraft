#ifndef DRK_GRAPHICS_OPEN_GL_CONTEXT_HPP
#define DRK_GRAPHICS_OPEN_GL_CONTEXT_HPP

#include "Core/Base.hpp"

#include "lib/string.hpp"
#include "lib/string_view.hpp"

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

        string_view get_gl_version(void) const;
        string_view get_renderer_info(void) const;

    private:
        void get_context_info(void);

    private:
        Window& m_window;

        string m_glVersion;
        string m_rendererHardware;
    };
}

#endif // DRK_GRAPHICS_OPEN_GL_CONTEXT_HPP
