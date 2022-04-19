#include "Renderer.hpp"

#include "Graphics/Renderer/BlockRenderer.hpp"
#include "Graphics/Renderer/MeshRenderer.hpp"
#include "Graphics/Renderer/TextRenderer.hpp"
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

        s_textureManager = make_ptr<TextureManager>();

        BlockRenderer::init(s_textureManager);
        MeshRenderer::init();
        TextRenderer::init();
    }

    void Renderer::shutdown(void)
    {
        DRK_PROFILE_FUNCTION();

        TextRenderer::shutdown();
        MeshRenderer::shutdown();
        BlockRenderer::shutdown();

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

    void Renderer::attach_texture(const Texture& texture)
    {

    }

    void Renderer::detach_texture(const Texture& texture)
    {

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
        s_data.lastStats = s_data.stats;
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
