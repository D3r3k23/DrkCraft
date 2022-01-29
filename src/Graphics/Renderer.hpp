#ifndef DRK_RENDERER_HPP
#define DRK_RENDERER_HPP

#include "Core/Base.hpp"
#include "Core/BuildSettings.hpp"
#include "OpenGlContext.hpp"
#include "VertexArray.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <glm/vec2.hpp>

namespace DrkCraft
{
    struct RendererStats
    {
        uint drawCalls = 0;
        uint vertices  = 0; // ?
        uint indices   = 0;
        uint textures  = 0;
    };

    class Renderer
    {
    public:
        static void init(OpenGlContext& context, const glm::uvec2& viewportSize);
        static void shutdown(void);

        struct SceneData
        {
            Camera camera;
        };

        static void begin_frame(void);
        static void end_frame(void);

        static void begin_scene(const SceneData& data);
        static void end_scene(void);

        static void attach_texture(const Ref<Texture>& texture);

        static const Camera& get_camera(void);

        //////////////////////////////////////////////

        // static void draw_block(uint x, uint y, uint z);

        static void draw_triangle(VertexBuffer& vbo);

        // static void draw_cube_mesh(const CubeMesh& mesh); // ??

        //////////////////////////////////////////////////

        static void set_viewport(int x, int y, uint width, uint height);
        static void set_viewport(const glm::ivec2& pos, const glm::uvec2& size);

        static const RendererStats& get_stats(void);
        static void reset_stats(void);

    private:
        static void clear(void);

        static void draw(const Ref<VertexArray>& vao);
        static void draw_indexed(const Ref<IndexBuffer>& indexBuffer, uint count=0);
    };
}

#endif // DRK_RENDERER_HPP
