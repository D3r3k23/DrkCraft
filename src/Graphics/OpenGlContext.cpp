#include "OpenGlContext.hpp"

#include "Core/BuildSettings.hpp"
#include "Core/Profiler.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
#if DRK_LOGGING_ENABLED
    void GLAPIENTRY gl_message_handler(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* msg, const void* userParam);
#endif

    OpenGlContext::OpenGlContext(Window& window)
      : m_window(window.get_raw_window())
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Initializing OpenGL");
        {
            DRK_PROFILE_SCOPE("glfwMakeContextCurrent");
            glfwMakeContextCurrent(m_window);
        }
        DRK_LOG_CORE_TRACE("Loading Glad OpenGL using GLFW loader function");
        {
            DRK_PROFILE_SCOPE("Load GL Loader");
            int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            DRK_ASSERT_CORE(status, "Glad failed to initialize OpenGL context");
        }
        const auto* version = glGetString(GL_VERSION);
        DRK_LOG_CORE_INFO("OpenGL version: {}", version);

    #if DRK_LOGGING_ENABLED
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(gl_message_handler, nullptr);
    #endif
    }

    OpenGlContext::~OpenGlContext(void)
    {

    }

    void OpenGlContext::swap_buffers(void)
    {
        DRK_PROFILE_FUNCTION();
        glfwSwapBuffers(m_window);
    }

#if DRK_LOGGING_ENABLED
    void GLAPIENTRY gl_message_handler(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* userParam)
    {
        if (id == 131169 || id == 131185 || id == 131218 || 131204)
            return;

        const char* sourceStr = [source]{
            switch (source)
            {
                case GL_DEBUG_SOURCE_API             : return "API";
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM   : return "window system";
                case GL_DEBUG_SOURCE_SHADER_COMPILER : return "shader compiler";
                case GL_DEBUG_SOURCE_THIRD_PARTY     : return "third party";
                case GL_DEBUG_SOURCE_APPLICATION     : return "application";
                case GL_DEBUG_SOURCE_OTHER           : return "other";
                default                              : return "unknown";
            }
        }();
        const char* typeStr = [type]{
            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR               : return "error";
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR : return "deprecated behavior";
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  : return "undefined behavior";
                case GL_DEBUG_TYPE_PORTABILITY         : return "portability";
                case GL_DEBUG_TYPE_PERFORMANCE         : return "performance";
                case GL_DEBUG_TYPE_MARKER              : return "marker";
                case GL_DEBUG_TYPE_PUSH_GROUP          : return "push group";
                case GL_DEBUG_TYPE_POP_GROUP           : return "pop group";
                case GL_DEBUG_TYPE_OTHER               : return "other";
                default                                : return "unknown";
            }
        }();
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                DRK_LOG_CORE_ERROR("[OpenGL error] source: {0} | type: {1} | msg: {2}", sourceStr, typeStr, msg);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                DRK_LOG_CORE_WARN("[OpenGL error] source: {0} | type: {1} | msg: {2}", sourceStr, typeStr, msg);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                DRK_LOG_CORE_INFO("[OpenGL error] source: {0} | type: {1} | msg: {2}", sourceStr, typeStr, msg);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                DRK_LOG_CORE_INFO("[OpenGL message] source: {0} | type: {1} | msg: {2}", sourceStr, typeStr, msg);
                break;
            default:
                DRK_LOG_CORE_WARN("[OpenGL message] severity: {0} | source: {1} | type: {2} | msg: {3}", severity, sourceStr, typeStr, msg);
        }
    }
#endif
}
