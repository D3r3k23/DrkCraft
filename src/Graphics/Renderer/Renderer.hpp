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
        uint vertices  = 0; // ?
        uint indices   = 0;
        uint textures  = 0;
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

    private:
        static void reset_stats(void);
        static void clear(void);

    public:
        class Passkey
        {
        private:
            Passkey(void) = default;
            friend class Renderer;
            friend class CubeRenderer;
            friend class MeshRenderer;
            friend class TextRenderer;
        };

        static void bind_shader(Passkey, const ShaderProgram& shader);
        static void unbind_shader(Passkey, const ShaderProgram& shader);

        static void attach_texture(Passkey, const Texture& texture);
        static void detach_texture(Passkey, const Texture& texture);

        static void draw_indexed(Passkey, const VertexArray& vao);
        static void draw_triangles(Passkey, const IndexBuffer& indexBuffer, std::optional<uint> count=std::nullopt);
    };
}

#endif // DRK_GRAPHICS_RENDERER_RENDERER_HPP