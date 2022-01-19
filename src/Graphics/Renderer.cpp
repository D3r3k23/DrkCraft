#include "Renderer.hpp"

#include "CubeRenderer.hpp"
#include "Util.hpp"
#include "Core/Profiler.hpp"

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include <array>

namespace DrkCraft
{
    struct RendererData
    {
        RendererStats stats;

        GlObjectID vao;

        Ptr<CubeRenderer> cubeRenderer;
    };

    static RendererData s_rendererData;

    void Renderer::init(OpenGlContext& context, const glm::uvec2& viewportSize)
    {
        DRK_PROFILE_FUNCTION();

        const auto* hardware = glGetString(GL_RENDERER);
        DRK_LOG_CORE_INFO("Renderer hardware: {}", hardware);

        set_viewport({0, 0}, viewportSize);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        clear();
        context.swap_buffers();
        {
            DRK_PROFILE_SCOPE("Initialize VAO");
            glGenVertexArrays(1, &s_rendererData.vao);
            glBindVertexArray(s_rendererData.vao);
        }
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

    void Renderer::draw_indexed(const IndexBuffer& buffer)
    {
        glDrawElements(GL_TRIANGLES, buffer.get_count(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::draw_triangle(VertexBuffer& vbo)
    {
        DRK_PROFILE_FUNCTION();

        GlObjectHandler<VertexBuffer> vboHandler(vbo);
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

    void Renderer::set_viewport(const glm::ivec2& pos, const glm::uvec2& size)
    {
        set_viewport(pos.x, pos.y, size.x, size.y);
    }

    const RendererStats& get_stats(void)
    {
        return s_rendererData.stats;
    }
}
