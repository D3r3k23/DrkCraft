#ifndef DRK_RENDERER_HPP
#define DRK_RENDERER_HPP

#include "Core/Base.hpp"
#include "Core/BuildSettings.hpp"
#include "Mesh.hpp"

// Temp?
#include <glad/glad.h>

namespace DrkCraft
{
    struct RendererStats
    {
        uint drawCalls = 0;
        uint triangles = 0;
        uint quads = 0;
    };

    class Renderer
    {
    public:
        static void init(void);
        static void shutdown(void);

        static void begin_frame(void);
        static void end_frame(void);

        static void draw_block(uint x, uint y, uint z);

        static void draw_triangle(GLuint vao);

        static void draw_cube_mesh(const CubeMesh& mesh); // ??

        static void set_viewport(int x, int y, uint width, uint height);

        static const RendererStats& get_stats(void);

    private:
    };

#if defined(DRK_EN_LOGGING)
    void register_gl_message_handler(void);
#endif
}

#endif // DRK_RENDERER_HPP
