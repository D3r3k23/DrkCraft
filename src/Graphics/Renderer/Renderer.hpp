#ifndef DRK_GRAPHICS_RENDERER_RENDERER_HPP
#define DRK_GRAPHICS_RENDERER_RENDERER_HPP

#include "Core/Base.hpp"
#include "Graphics/OpenGlContext.hpp"
#include "Graphics/detail/VertexArray.hpp"
#include "Graphics/detail/Buffer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Camera.hpp"

#include "lib/glm/vec.hpp"

#include <vector>
#include <optional>

namespace DrkCraft
{
    struct RendererStats
    {
        uint drawCalls = 0;
        uint indices   = 0;
        uint lines     = 0;
        uint triangles = 0;
    };

    struct LightSource // Somewhere else
    {
        vec3 position;
        vec3 direction;
        vec4 color;
    };

    struct SceneData
    {
        Camera camera;
        std::vector<LightSource> lights;
    };

    class Renderer
    {
    public:
        static void init(OpenGlContext& context, const uvec2& viewportSize);
        static void shutdown(void);

        static void begin_frame(void);
        static void end_frame(void);

        static void begin_scene(const SceneData& data);
        static void end_scene(void);

        static const Camera& get_camera(void);

        static void set_viewport(int x, int y, uint width, uint height);
        static void set_viewport(const ivec2& pos, const uvec2& size);

        static const RendererStats& get_stats(void);

    protected:
        static void bind_shader(const ShaderProgram& shader);
        static void unbind_shader(const ShaderProgram& shader);

        static void attach_texture(const Texture& texture);
        static void detach_texture(const Texture& texture);

        static void draw_indexed(const VertexArray& vao, std::optional<uint> count={});

    private:
        static void reset_stats(void);
        static void clear(void);

        static void update_stats_on_draw_call(PrimitiveType primitive, uint indices);
    };
}

#endif // DRK_GRAPHICS_RENDERER_RENDERER_HPP
