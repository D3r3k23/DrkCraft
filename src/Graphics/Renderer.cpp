#include "Renderer.hpp"

#include "CubeRenderer.hpp"
#include "Core/Profiler.hpp"

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include <array>

namespace DrkCraft
{
    struct RendererData
    {
        RendererStats stats;

        Ptr<CubeRenderer> cubeRenderer;
    };

    static RendererData s_rendererData;

    void Renderer::init(void)
    {
        DRK_PROFILE_FUNCTION();

        const auto* renderer = glGetString(GL_RENDERER);
        DRK_LOG_CORE_INFO("Renderer: {}", renderer);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        s_rendererData.cubeRenderer = make_ptr<CubeRenderer>();
    }

    void Renderer::shutdown(void)
    {
        DRK_PROFILE_FUNCTION();
    }

    void Renderer::begin_frame(void)
    {
        clear();
    }

    void Renderer::end_frame(void)
    {

    }

    void Renderer::clear(void)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::draw_triangle(GLuint vao)
    {
        DRK_PROFILE_FUNCTION();

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void Renderer::draw_block(uint x, uint y, uint z)
    {
        s_rendererData.cubeRenderer->draw_cube(Transform::Identity());
    }

    void Renderer::draw_cube_mesh(const CubeMesh& mesh)
    {
        DRK_PROFILE_FUNCTION();

        auto vertices = mesh.get_vertices();


    }

    void Renderer::set_viewport(int x, int y, uint width, uint height)
    {
        glViewport(x, y, width, height);
    }

    const RendererStats& get_stats(void)
    {
        return s_rendererData.stats;
    }
}
