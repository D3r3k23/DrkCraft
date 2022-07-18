#ifndef DRK_GRAPHICS_RENDERER_MESH_RENDERER
#define DRK_GRAPHICS_RENDERER_MESH_RENDERER

#include "Core/Base.hpp"
#include "Graphics/Renderer/Renderer.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Transform.hpp"

namespace DrkCraft
{
    struct MeshRendererStats
    {
        uint meshes = 0;
    };

    class MeshRenderer : public Renderer
    {
    public:
        static void init(void);
        static void shutdown(void);

        static void begin_frame(void);
        static void end_frame(void);

        static void begin_scene(void);
        static void end_scene(void);

        static void submit(const Mesh& mesh, const Transform& transform);

        static const MeshRendererStats& get_stats(void);

    private:
        static void reset_stats(void);
    };
}

#endif // DRK_GRAPHICS_RENDERER_MESH_RENDERER
