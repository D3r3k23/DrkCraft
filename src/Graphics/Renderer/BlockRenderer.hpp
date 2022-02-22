#ifndef DRK_GRAPHICS_RENDERER_BLOCK_RENDERER_HPP
#define DRK_GRAPHICS_RENDERER_BLOCK_RENDERER_HPP

#include "Core/Base.hpp"
#include "Graphics/Texture.hpp"

#include "lib/glm/vec2.hpp"
#include "lib/glm/vec3.hpp"

#include <vector>

namespace DrkCraft
{
    struct BlockRendererStats
    {
        uint blocks     = 0;
        uint blockFaces = 0;
    };

    class BlockRenderer
    {
    public:
        static void init(const Ptr<TextureManager>& textureSlots);
        static void shutdown(void);

        static void set_texture_atlas(const Ref<Texture>& atlasTexture);

        static void begin_scene(void);
        static void end_scene(void);

        static void submit(const ivec3& position, uint tid);

        static const BlockRendererStats& get_stats(void);
        static void reset_stats(void);

    private:
        static void start_batch(void);
        static void next_batch(void);

        static void flush(void);
    };
}

#endif // DRK_GRAPHICS_RENDERER_BLOCK_RENDERER_HPP
