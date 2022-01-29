#include "Renderer.hpp"

#include "CubeRenderer.hpp"
#include "TextureManager.hpp"
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
        Renderer::SceneData sceneData;
        TextureManager textureSlots;
    };

    static RendererData s_data;

    void Renderer::init(OpenGlContext& context, const glm::uvec2& viewportSize)
    {
        DRK_PROFILE_FUNCTION();

        const auto* hardware = glGetString(GL_RENDERER);
        DRK_LOG_CORE_INFO("Renderer hardware: {}", hardware);

        set_viewport({0, 0}, viewportSize);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        clear();
        context.swap_buffers();

        CubeRenderer::init();
    }

    void Renderer::shutdown(void)
    {
        DRK_PROFILE_FUNCTION();

        CubeRenderer::shutdown();
    }

    void Renderer::begin_frame(void)
    {
        clear();
        reset_stats();
    }

    void Renderer::end_frame(void)
    {

    }

    void Renderer::begin_scene(const SceneData& data)
    {
        s_data.sceneData = data;
    }

    void Renderer::end_scene(void)
    {

    }

    void Renderer::attach_texture(const Ref<Texture>& texture)
    {
        if ()
        s_data.textureSlots.add(texture);
    }

    const Camera& Renderer::get_camera(void)
    {
        return s_data.sceneData.camera;
    }

    void Renderer::clear(void)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::draw_indexed(const Ref<IndexBuffer>& buffer, uint count)
    {
        if (count == 0 || count > buffer->get_count())
            count = buffer->get_count();

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        s_data.stats.drawCalls++;
    }

    void Renderer::draw_triangle(VertexBuffer& vbo)
    {
        DRK_PROFILE_FUNCTION();

        GlObjectHandler<VertexBuffer> vboHandler(vbo);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void Renderer::draw_block(uint x, uint y, uint z)
    {
        s_data.cubeRenderer->draw_cube(Transform::Identity());
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

    const RendererStats& Renderer::get_stats(void)
    {
        return s_data.stats;
    }

    void Renderer::reset_stats(void)
    {
        s_data.stats = RendererStats{};
    }
}
