#ifndef DRK_GRAPHICS_RENDERER_BLOCK_RENDERER_HPP
#define DRK_GRAPHICS_RENDERER_BLOCK_RENDERER_HPP

#include "Core/Base.hpp"
#include "Graphics/Renderer/Renderer.hpp"
#include "Graphics/Texture.hpp"

#include "Lib/glm/vec2.hpp"
#include "Lib/glm/vec3.hpp"

namespace DrkCraft
{
    struct BlockRendererStats
    {
        uint blocks     = 0;
        uint blockFaces = 0;
    };

    class BlockRenderer : private Renderer
    {
    public:
        static void init(TextureManager* textureManager);
        static void shutdown(void);

        static void set_texture_atlas(const Ref<Texture>& atlasTexture);

        static void begin_frame(void);
        static void end_frame(void);

        static void begin_scene(void);
        static void end_scene(void);

        static void submit(const ivec3& position, uint tid);

        static const BlockRendererStats& get_stats(void);

    private:
        static void reset_stats(void);

        static void draw_batch(void);
        static void flush_batch(void);

        static void update_vertex_buffer(void);
        static void update_index_buffer(void);
    };
}

#endif // DRK_GRAPHICS_RENDERER_BLOCK_RENDERER_HPP
