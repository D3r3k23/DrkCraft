#include "Renderer.hpp"

#include "Graphics/Renderer/CubeRenderer.hpp"
#include "Graphics/detail/Util.hpp"
#include "Core/Debug/Profiler.hpp"

#include <glad/glad.h>

#include <algorithm>

namespace DrkCraft
{
    namespace
    {
        struct RendererData
        {
            RendererStats lastStats;
            RendererStats stats;

            SceneData sceneData;
        };
    }
    static RendererData s_data;

    Ptr<TextureManager> s_textureManager;

    void Renderer::init(OpenGlContext& context, const uvec2& viewportSize)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Initializing core Renderer");

        DRK_LOG_CORE_INFO("Renderer hardware: {}", context.get_renderer_info());

        set_viewport({0, 0}, viewportSize);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // clear();
        // context.swap_buffers();

        s_textureManager = make_ptr<TextureManager>();
        CubeRenderer::init(s_textureManager);
    }

    void Renderer::shutdown(void)
    {
        DRK_PROFILE_FUNCTION();

        CubeRenderer::shutdown();
        s_textureManager.reset();
    }

    void Renderer::begin_frame(void)
    {
        clear();
    }

    void Renderer::end_frame(void)
    {

    }

    void Renderer::begin_scene(const SceneData& data)
    {
        s_data.sceneData = data;
        reset_stats();
    }

    void Renderer::end_scene(void)
    {
        s_data.lastStats = s_data.stats;
    }

    const Camera& Renderer::get_camera(void)
    {
        return s_data.sceneData.camera;
    }

    void Renderer::set_viewport(int x, int y, uint width, uint height)
    {
        glViewport(x, y, width, height);
    }

    void Renderer::set_viewport(const ivec2& pos, const uvec2& size)
    {
        set_viewport(pos.x, pos.y, size.x, size.y);
    }

    const RendererStats& Renderer::get_stats(void)
    {
        return s_data.lastStats;
    }

    void Renderer::reset_stats(void)
    {
        s_data.lastStats = s_data.stats;
        s_data.stats = RendererStats{};
    }

    void Renderer::clear(void)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::attach_texture(Passkey, const Texture& texture)
    {

    }

    void Renderer::detach_texture(Passkey, const Texture& texture)
    {

    }

    void Renderer::draw_indexed(Passkey, const VertexArray& vao)
    {
        DRK_PROFILE_FUNCTION();

        vao.bind();
        glDrawElements(
            to_gl_primitive_type(vao.get_vertex_buffer()->get_primitive_type()),
            vao.get_index_buffer()->get_count(),
            GL_UNSIGNED_INT, nullptr);
        vao.unbind();

        s_data.stats.drawCalls++;
    }

    void Renderer::draw_triangles(Passkey, const IndexBuffer& indexBuffer, std::optional<uint> count)
    {
        DRK_PROFILE_FUNCTION();

        uint indexCount = (!count || *count > indexBuffer.get_count()) ? indexBuffer.get_count() : *count;

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

        s_data.stats.indices += indexCount;
        s_data.stats.drawCalls++;
    }
}
