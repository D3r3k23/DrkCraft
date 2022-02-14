#include "OpenGlContext.hpp"

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

        DRK_LOG_CORE_INFO("OpenGL initialized");

        const auto* version = glGetString(GL_VERSION);
        DRK_LOG_CORE_INFO("OpenGL version: {}", version);

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
}
