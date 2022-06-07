#include "Renderer.hpp"

#include "Graphics/Renderer/BlockRenderer.hpp"
#include "Graphics/Renderer/MeshRenderer.hpp"
#include "Graphics/Renderer/TextRenderer.hpp"
#include "Graphics/detail/Buffer.hpp"
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
            RendererStats lastFrameStats;
            RendererStats stats;

            Scene scene;

            Ptr<TextureManager> textureManager;
        };
    }

    static RendererData s_data;

    void Renderer::init(OpenGlContext& context, const uvec2& viewportSize)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Initializing Renderer");

        DRK_LOG_CORE_INFO("Renderer hardware: {}", context.get_renderer_info());

        set_viewport({0, 0}, viewportSize);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // clear();
        // context.swap_buffers();

        s_data.textureManager = make_ptr<TextureManager>();

        BlockRenderer::init(s_data.textureManager.get());
        MeshRenderer::init();
        TextRenderer::init();
    }

    void Renderer::shutdown(void)
    {
        DRK_PROFILE_FUNCTION();

        TextRenderer::shutdown();
        MeshRenderer::shutdown();
        BlockRenderer::shutdown();

        s_data.textureManager.reset();
    }

    void Renderer::set_viewport(int x, int y, uint width, uint height)
    {
        glViewport(x, y, width, height);
    }

    void Renderer::set_viewport(const ivec2& pos, const uvec2& size)
    {
        set_viewport(pos.x, pos.y, size.x, size.y);
    }

    void Renderer::begin_frame(void)
    {
        reset_stats();
        clear();

        BlockRenderer::begin_frame();
        MeshRenderer::begin_frame();
        TextRenderer::begin_frame();
    }

    void Renderer::end_frame(void)
    {
        TextRenderer::end_frame();
        MeshRenderer::end_frame();
        BlockRenderer::end_frame();
    }

    void Renderer::begin_scene(const Camera& camera)
    {
        s_data.scene.set_camera(camera);
        begin_scene();
    }

    void Renderer::begin_scene(void)
    {
        s_data.scene.reset_lights();
    }

    void Renderer::end_scene(void)
    {

    }

    Scene& Renderer::get_scene(void)
    {
        return s_data.scene;
    }

    const RendererStats& Renderer::get_stats(void)
    {
        return s_data.lastFrameStats;
    }

    void Renderer::draw_indexed(const VertexArray& vao, std::optional<uint> count)
    {
        DRK_PROFILE_FUNCTION();

        const auto indexBuffer = vao.get_index_buffer();
        uint indexCount = std::min(count.value_or(indexBuffer->get_count()), indexBuffer->get_count());

        const auto primitiveType = vao.get_vertex_buffer()->get_primitive_type();
        const auto glPrimitiveType = to_gl_primitive_type(primitiveType);

        vao.bind();
        glDrawElements(glPrimitiveType, indexCount, GL_UNSIGNED_INT, nullptr);

        update_stats_on_draw_call(primitiveType, indexCount);
    }

    // void Renderer::draw_triangles(const IndexBuffer& indexBuffer, std::optional<uint> count)
    // {
    //     DRK_PROFILE_FUNCTION();

    //     uint indexCount = (!count || *count > indexBuffer.get_count()) ? indexBuffer.get_count() : *count;

    //     glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

    //     s_data.stats.indices += indexCount;
    //     s_data.stats.drawCalls++;
    // }

    void Renderer::reset_stats(void)
    {
        s_data.lastFrameStats = s_data.stats;
        s_data.stats = RendererStats{};
    }

    void Renderer::clear(void)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::update_stats_on_draw_call(PrimitiveType primitive, uint indices)
    {
        s_data.stats.drawCalls++;
        s_data.stats.indices += indices;

        using enum PrimitiveType;
        switch (primitive)
        {
            case Lines     : s_data.stats.lines += indices / 2; break;
            case LineStrip : s_data.stats.lines += indices - 1; break;
            case LineLoop  : s_data.stats.lines += indices;     break;

            case Triangles     : s_data.stats.triangles += indices / 3; break;
            case TriangleStrip : s_data.stats.triangles += indices - 2; break;
            case TriangleFan   : s_data.stats.triangles += 0;           break;
        }
    }
}
