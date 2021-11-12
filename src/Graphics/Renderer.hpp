#ifndef DRK_RENDERER_HPP
#define DRK_RENDERER_HPP

#include "Core/Base.hpp"
#include "Core/BuildSettings.hpp"

#include <glad/glad.h>
#include <glm/vec3.hpp>

namespace DrkCraft
{
    struct RendererStats
    {
        uint numQuads = 0;
    };

    class Renderer
    {
    public:
        static void init(void);
        static void shutdown(void);

        static void begin_frame(void);
        static void end_frame(void);

        static void draw_triangle(GLuint vao);

        static void set_viewport(int x, int y, int width, int height);

        static const RendererStats& get_stats(void);

    private:

    };

#if defined(DRK_EN_LOGGING)
    void register_gl_message_handler(void);
    void GLAPIENTRY gl_message_handler(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* userParam);
#endif
}

#endif // DRK_RENDERER_HPP
