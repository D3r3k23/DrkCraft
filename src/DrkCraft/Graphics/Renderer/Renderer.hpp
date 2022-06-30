#ifndef DRK_GRAPHICS_RENDERER_RENDERER_HPP
#define DRK_GRAPHICS_RENDERER_RENDERER_HPP

#include "Core/Base.hpp"
#include "Graphics/OpenGlContext.hpp"
#include "Graphics/detail/VertexArray.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Scene.hpp"
#include "Graphics/Camera.hpp"

#include "lib/glm/vec.hpp"

namespace DrkCraft
{
    struct RendererStats
    {
        uint drawCalls = 0;
        uint vertices  = 0;
        uint indices   = 0;
        uint lines     = 0;
        uint triangles = 0;
    };

    static constexpr uint WHITE_TEXTURE_SLOT = 0;

    class Renderer
    {
    public:
        static void init(const OpenGlContext& context, const uvec2& viewportSize);
        static void shutdown(void);

        static void set_viewport(int x, int y, uint width, uint height);
        static void set_viewport(const ivec2& pos, const uvec2& size);

        static void clear(void);

        static void begin_frame(void);
        static void end_frame(void);

        static void begin_scene(const Camera& camera);
        static void begin_scene(void);
        static void end_scene(void);

        static Scene& get_scene(void);

        static const RendererStats& get_stats(void);

    protected:
        static void bind_shader(const ShaderProgram& shader);
        static void unbind_shader(const ShaderProgram& shader);

        static void draw_indexed(const VertexArray& vao, Optional<uint> count={});
    };
}

#endif // DRK_GRAPHICS_RENDERER_RENDERER_HPP
