#include "MeshRenderer.hpp"

namespace DrkCraft
{
    namespace
    {
        struct MeshRendererData
        {
            MeshRendererStats lastStats;
            MeshRendererStats stats;
        };
    }

    static MeshRendererData s_data;

    void MeshRenderer::init(void)
    {

    }

    void MeshRenderer::shutdown(void)
    {

    }

    void MeshRenderer::begin_frame(void)
    {

    }

    void MeshRenderer::end_frame(void)
    {

    }

    void MeshRenderer::begin_scene(void)
    {

    }

    void MeshRenderer::end_scene(void)
    {

    }

    void MeshRenderer::submit(const Mesh& mesh, const Transform& transform)
    {

    }

    const MeshRendererStats& MeshRenderer::get_stats(void)
    {
        return s_data.lastStats;
    }
}
