#include "OpenGlContext.hpp"

#include "System/Window.hpp"
#include "Graphics/detail/GlTools.hpp"
#include "Core/Debug/Profiler.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace DrkCraft
{
    OpenGlContext::OpenGlContext(Window& window)
      : m_window(window)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Initializing OpenGL context");

        DRK_LOG_CORE_TRACE("Setting current OpenGL context");
        make_current();

        DRK_LOG_CORE_TRACE("Loading Glad OpenGL using GLFW loader function");
        load_opengl();

        get_context_info();
        DRK_LOG_CORE_INFO("OpenGL initialized");
        DRK_LOG_CORE_INFO("OpenGL version: {}", m_glVersion);

        if constexpr (DRK_LOGGING_ENABLED)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(gl_message_handler, nullptr);
            DRK_LOG_CORE_INFO("Enabled logging OpenGL debug messages");
        }
    }

    OpenGlContext::~OpenGlContext(void)
    {
        clear_current();
    }

    void OpenGlContext::swap_buffers(void)
    {
        DRK_PROFILE_FUNCTION();
        glfwSwapBuffers(m_window.get_raw_window());
    }

    void OpenGlContext::make_current(void)
    {
        DRK_PROFILE_FUNCTION();
        glfwMakeContextCurrent(m_window.get_raw_window());
    }

    void OpenGlContext::clear_current(void)
    {
        glfwMakeContextCurrent(nullptr);
    }

    std::string_view OpenGlContext::get_gl_version(void) const
    {
        return m_glVersion;
    }

    std::string_view OpenGlContext::get_renderer_info(void) const
    {
        return m_rendererHardware;
    }

    void OpenGlContext::get_context_info(void)
    {
        const auto* version = glGetString(GL_VERSION);
        m_glVersion = (const char*)version;

        const auto* renderer = glGetString(GL_RENDERER);
        m_rendererHardware = (const char*)renderer;
    }
}
