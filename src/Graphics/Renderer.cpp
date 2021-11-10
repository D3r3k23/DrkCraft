#include "Renderer.hpp"

#include "Core/BuildSettings.hpp"
#include "Application/Application.hpp"

#include <unordered_map>

namespace DrkCraft
{
    struct RendererData
    {
        RendererStats stats;
    };

    static RendererData s_rendererData;

    void Renderer::init(void)
    {
        #if defined (DRK_EN_GL_DEBUG_OUTPUT)
            register_gl_message_handler();
        #endif

        const auto* renderer = glGetString(GL_RENDERER);
        const auto* version  = glGetString(GL_VERSION);
        DRK_LOG_INFO("Renderer: {}", renderer);
        DRK_LOG_INFO("OpenGL version: {}", version);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    void Renderer::shutdown(void)
    {

    }

    void Renderer::begin_frame(void)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::end_frame(void)
    {

    }

    void Renderer::draw_triangle(GLuint vao)
    {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    const RendererStats& get_stats(void)
    {
        return s_rendererData.stats;
    }

    void Renderer::register_gl_message_handler(void)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(gl_message_handler, nullptr);
    }

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
            case GL_DEBUG_SEVERITY_HIGH         : DRK_LOG_ERROR("[OpenGL error] source: {0} type: {1} msg: {2}",   sourceStr, typeStr, msg); break;
            case GL_DEBUG_SEVERITY_MEDIUM       : DRK_LOG_WARN ("[OpenGL error] source: {0} type: {1} msg: {2}",   sourceStr, typeStr, msg); break;
            case GL_DEBUG_SEVERITY_LOW          : DRK_LOG_INFO ("[OpenGL error] source: {0} type: {1} msg: {2}",   sourceStr, typeStr, msg); break;
            case GL_DEBUG_SEVERITY_NOTIFICATION : DRK_LOG_INFO ("[OpenGL message] source: {0} type: {1} msg: {2}", sourceStr, typeStr, msg); break;
            default                             : DRK_LOG_WARN ("[OpenGL message] severity: {0} source: {1} type: {2} msg: {3}", severity, sourceStr, typeStr, msg);
        }
    }
}
