#ifndef DRK_GRAPHICS_RENDERER_CUBE_RENDERER_HPP
#define DRK_GRAPHICS_RENDERER_CUBE_RENDERER_HPP

#include "Core/Base.hpp"
#include "Graphics/Texture.hpp"

#include "lib/glm/vec2.hpp"
#include "lib/glm/vec3.hpp"

#include <vector>

namespace DrkCraft
{
    struct CubeRendererStats
    {
        uint cubes     = 0;
        uint cubeFaces = 0;
    };

    class CubeRenderer
    {
    public:
        static void init(const Ptr<TextureManager>& textureSlots);
        static void shutdown(void);

        static void set_texture_atlas(const TextureAtlas& atlas);

        static void begin_scene(void);
        static void end_scene(void);

        static void submit(const ivec3& position, const Ref<Texture>& texture, const vec4& color);
        static void submit(const ivec3& position, const Ref<Texture>& texture);
        static void submit(const ivec3& position, const vec4& color);

        static const CubeRendererStats& get_stats(void);
        static void reset_stats(void);

    private:
        static void start_batch(void);
        static void next_batch(void);

        static void flush(void);
    };
}

#endif // DRK_GRAPHICS_RENDERER_CUBE_RENDERER_HPP
