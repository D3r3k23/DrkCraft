#ifndef DRK_CUBE_RENDERER_HPP
#define DRK_CUBE_RENDERER_HPP

#include "Core/Base.hpp"
#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "Transform.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

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
        static void init(void);
        static void shutdown(void);

        static void begin_scene(void);
        static void end_scene(void);

        static void submit(const glm::ivec3& position, const Ref<Texture2D>& texture, const glm::vec4& color);
        static void submit(const glm::ivec3& position, const Ref<Texture2D>& texture);
        static void submit(const glm::ivec3& position, const glm::vec4& color);

        static const CubeRendererStats& get_stats(void);
        static void reset_stats(void);

    private:
        static void start_batch(void);
        static void next_batch(void);

        static void flush(void);
    };
}

#endif // DRK_CUBE_RENDERER_HPP
