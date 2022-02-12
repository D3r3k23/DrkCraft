#ifndef DRK_GRAPHICS_RENDERER_HPP
#define DRK_GRAPHICS_RENDERER_HPP

#include "Core/Base.hpp"
#include "Graphics/OpenGlContext.hpp"
#include "Graphics/detail/VertexArray.hpp"
#include "Graphics/detail/Buffer.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

#include "lib/glm/vec.hpp"

#include <vector>
#include <optional>

namespace DrkCraft
{
    struct RendererStats
    {
        uint drawCalls = 0;
        uint vertices  = 0; // ?
        uint indices   = 0;
        uint textures  = 0;
    };

    struct Light
    {
        vec3 position;
        vec3 direction;
        vec4 color;
    };

    struct SceneData
    {
        Camera camera;
        std::vector<Light> lights;
    };

    class Renderer
    {
        friend class CubeRenderer;
        friend class MeshRenderer;

    public:
        static void init(OpenGlContext& context, const uvec2& viewportSize);
        static void shutdown(void);

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
        static void set_viewport(const ivec2& pos, const uvec2& size);

        static const RendererStats& get_stats(void);
        static void reset_stats(void);

    private:
        static void clear(void);

        static void draw(const Ref<VertexArray>& vao);
        static void draw_indexed(const Ref<VertexArray>& vao);
        static void draw_indexed(const Ref<IndexBuffer>& indexBuffer, std::optional<uint> indexCount=std::nullopt);
    };
}

#endif // DRK_GRAPHICS_RENDERER_HPP
