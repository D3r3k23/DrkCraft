#include "Renderer.hpp"

#include "Graphics/Renderer/BlockRenderer.hpp"
#include "Graphics/Renderer/MeshRenderer.hpp"
#include "Graphics/Renderer/TextRenderer.hpp"
#include "Graphics/Texture.hpp"
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

    namespace
    {
        Ref<Texture> create_white_texture(void);

        void reset_stats(void);
        void update_stats_on_draw_call(const VertexBuffer& vertexBuffer, uint indices);
    }

    void Renderer::init(OpenGlContext& context, const uvec2& viewportSize)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Initializing Renderer");

        DRK_LOG_CORE_INFO("Renderer hardware: {}", context.get_renderer_info());
        {
            DRK_PROFILE_SCOPE("Configure OpenGL");
            set_viewport({0, 0}, viewportSize);

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

            // clear();
            // context.swap_buffers();
        }{
            DRK_PROFILE_SCOPE("Initialize TextureManager");
            s_data.textureManager = make_ptr<TextureManager>();

            uint slot = s_data.textureManager->reserve();
            DRK_ASSERT_DEBUG(slot == WHITE_TEXTURE_SLOT, "White texture slot is not 0");

            DRK_LOG_CORE_TRACE("Creating white texture");
            s_data.textureManager->attach(create_white_texture(), slot);
        }
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

    void Renderer::clear(void)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

    void Renderer::draw_indexed(const VertexArray& vao, Optional<uint> count)
    {
        DRK_PROFILE_FUNCTION();

        const auto& vertexBuffer = *vao.get_vertex_buffer();
        const auto& indexBuffer = *vao.get_index_buffer();
        uint indexCount = std::min(count.value_or(indexBuffer.get_count()), indexBuffer.get_count());

        const auto primitiveType = vertexBuffer.get_primitive_type();
        const auto glPrimitiveType = to_gl_primitive_type(primitiveType);

        vao.bind();
        glDrawElements(glPrimitiveType, indexCount, GL_UNSIGNED_INT, nullptr);

        update_stats_on_draw_call(vertexBuffer, indexCount);
    }

    namespace
    {
        static Ref<Texture> create_white_texture(void)
        {
            static const uint32 WHITE_TEXTURE_DATA = 0xFFFFFFFF;
            static const uint8* whiteTextureDataPtr = reinterpret_cast<const uint8*>(&WHITE_TEXTURE_DATA);

            return Texture::from_data(whiteTextureDataPtr, {1, 1}, TextureFormat::RGBA);
        }

        void reset_stats(void)
        {
            s_data.lastFrameStats = s_data.stats;
            s_data.stats = RendererStats{};
        }

        void update_stats_on_draw_call(const VertexBuffer& vertexBuffer, uint indices)
        {
            s_data.stats.drawCalls += 1;
            s_data.stats.vertices += vertexBuffer.get_count();
            s_data.stats.indices += indices;

            const auto primitive = vertexBuffer.get_primitive_type();
            const auto category = get_category(primitive);
            if (category == PrimitiveCategory::Lines)
            {
                if (indices < 2)
                    return;
                else
                    switch (primitive)
                    {
                        case PrimitiveType::Lines     : s_data.stats.lines += indices / 2; return;
                        case PrimitiveType::LineStrip : s_data.stats.lines += indices - 1; return;
                        case PrimitiveType::LineLoop  : s_data.stats.lines += indices;     return;
                    }
            }
            else if (category == PrimitiveCategory::Triangles)
            {
                if (indices < 3)
                    return;
                else
                {
                    switch (primitive)
                    {
                        case PrimitiveType::Triangles     : s_data.stats.triangles += indices / 3; return;
                        case PrimitiveType::TriangleStrip : s_data.stats.triangles += indices - 2; return;
                        case PrimitiveType::TriangleFan   : s_data.stats.triangles += indices - 2; return;
                    }
                }
            }
            DRK_ASSERT_DEBUG_FALSE("Unknown PrimitiveType");
        }
    }
}
