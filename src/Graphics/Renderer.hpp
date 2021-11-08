#ifndef DRK_RENDERER_HPP
#define DRK_RENDERER_HPP

#include "Core/Base.hpp"
#include "Shader.hpp"

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

        static void add_shader_program(const ShaderProgram& program);

        static void begin(void);
        static void end(void);

        static void draw_triangle(glm::vec3 color, GLuint vao);

        static const RendererStats& get_stats(void);

    private:
        static void register_gl_message_handler(void);
    };

    void GLAPIENTRY gl_message_handler(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* userParam);
}

#endif // DRK_RENDERER_HPP
