#ifndef DRK_RENDERER_HPP
#define DRK_RENDERER_HPP

#include "Core/Base.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace DrkCraft
{
    struct RendererStats
    {

    };

    class Renderer
    {
    public:
        static void init(void);
        static const RendererStats& get_stats(void);

    private:
        static void register_gl_message_handler(void);
    };

    void GLAPIENTRY gl_message_handler(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* userParam);
}

#endif // DRK_RENDERER_HPP
