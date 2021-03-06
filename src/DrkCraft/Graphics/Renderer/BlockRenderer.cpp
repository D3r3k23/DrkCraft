#include "BlockRenderer.hpp"

#include "Graphics/Renderer/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/detail/VertexArray.hpp"
#include "Graphics/detail/Buffer.hpp"
#include "Graphics/detail/Util.hpp"
#include "System/Library.hpp"
#include "Core/Debug/Profiler.hpp"

#include <glad/glad.h>

#include <vector>

namespace DrkCraft
{
    static constexpr float BLOCK_SIZE = 0.5f;

    // Per batch
    static constexpr uint MAX_BLOCKS = 100;
    static constexpr uint NUM_VERTICES_IN_BLOCK = 12;
    static constexpr uint NUM_INDICES_IN_BLOCK  = 36;
    static constexpr uint MAX_BLOCK_VERTICES = MAX_BLOCKS * NUM_VERTICES_IN_BLOCK;
    static constexpr uint MAX_BLOCK_INDICES  = MAX_BLOCKS * NUM_INDICES_IN_BLOCK;

    namespace
    {
        struct BlockVertex
        {
            vec3 position;
            vec2 texCoord;
            uint texIndex;
        };

        struct BlockRendererData
        {
            BlockRendererStats lastFrameStats;
            BlockRendererStats stats;

            Optional<ShaderProgram> shader;
            Optional<VertexArray> vertexArray;

            Ref<VertexBuffer> vertexBuffer;
            Ref<IndexBuffer>  indexBuffer;

            std::vector<BlockVertex> vertexBufferData;
            uint batchSize = 0;

            TextureManager* textureManager;
            Optional<TextureAtlas> blockTextureAtlas;
            uint blockAtlasTextureIndex;
        };
    }

    static BlockRendererData s_data;

    void BlockRenderer::init(TextureManager* textureManager)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Initializing BlockRenderer");
        {
            DRK_PROFILE_SCOPE("BlockRenderer texture initialization");
            DRK_LOG_CORE_TRACE("Initializing BlockRenderer textures");
            s_data.textureManager = textureManager;

            s_data.blockAtlasTextureIndex = 0; // s_data.textureManager->reserve();
        }{
            DRK_PROFILE_SCOPE("BlockShader initialization");
            DRK_LOG_CORE_TRACE("Initializing BlockShader");
            s_data.shader.emplace("BlockShader");

            auto vert = Shader::create(shader_asset_path("block_vertex_shader.glsl"), ShaderType::Vertex);
            auto frag = Shader::create(shader_asset_path("block_fragment_shader.glsl"), ShaderType::Fragment);
            DRK_ASSERT_DEBUG(vert && frag, "Block shader compilation failed");

            s_data.shader->attach(vert);
            s_data.shader->attach(frag);
            s_data.shader->link();
        }
        const uint vertexBufferSize = MAX_BLOCK_VERTICES * sizeof(BlockVertex);
        const uint indexBufferSize = MAX_BLOCK_VERTICES * sizeof(Index);
        {
            DRK_PROFILE_SCOPE("BlockRenderer VertexArray initialization");
            DRK_LOG_CORE_TRACE("Initializing BlockRenderer VertexArray");
            s_data.vertexArray.emplace();
            {
                DRK_PROFILE_SCOPE("BlockRenderer VertexBuffer initialization");
                DRK_LOG_CORE_TRACE("Initializing BlockRenderer VertexBuffer");
                s_data.vertexBuffer = make_ref<VertexBuffer>(vertexBufferSize, PrimitiveType::Triangles);

                s_data.vertexBuffer->set_layout({
                    { ShaderDataType::Float3, "a_position" },
                    { ShaderDataType::Float2, "a_texCoord" },
                    { ShaderDataType::Uint,   "a_texIndex" }
                });

                s_data.vertexArray->set_vertex_buffer(s_data.vertexBuffer);
            }{
                DRK_PROFILE_SCOPE("BlockRenderer IndexBuffer initialization");
                DRK_LOG_CORE_TRACE("Initializing BlockRenderer IndexBuffer");
                s_data.indexBuffer = make_ref<IndexBuffer>(indexBufferSize);

                s_data.vertexArray->set_index_buffer(s_data.indexBuffer);
            }
        }
    }

    void BlockRenderer::shutdown(void)
    {
        s_data.vertexArray.reset();
        s_data.vertexBuffer.reset();
        s_data.shader.reset();
    }

#define NUM_BLOCKS 5

    void BlockRenderer::set_texture_atlas(const Ref<Texture>& atlasTexture)
    {
        s_data.blockTextureAtlas.emplace(atlasTexture, NUM_BLOCKS);
        s_data.textureManager->attach(atlasTexture, s_data.blockAtlasTextureIndex);
    }

    void BlockRenderer::begin_frame(void)
    {
        reset_stats();
    }

    void BlockRenderer::end_frame(void)
    {

    }

    void BlockRenderer::begin_scene(void)
    {
        const Camera& camera = Renderer::get_scene().get_camera();
        s_data.shader->bind();
        s_data.shader->upload_uniform("u_viewProjection", camera.get_view_projection());

        s_data.vertexArray->bind();
    }

    void BlockRenderer::end_scene(void)
    {
        if (s_data.batchSize > 0)
            draw();

        flush();

        s_data.vertexArray->unbind();
        s_data.shader->unbind();
    }

    void BlockRenderer::submit(const ivec3& position, uint tid)
    {
        DRK_PROFILE_FUNCTION();

        static constexpr vec3 BLOCK_VERTEX_POSITIONS[NUM_VERTICES_IN_BLOCK] = {
            {0.0f, 0.0f, 0.5f}, // 0
            {0.5f, 0.0f, 0.5f}, // 1
            {0.5f, 0.5f, 0.5f}, // 2
            {0.0f, 0.5f, 0.5f}, // 3
            {0.5f, 0.0f, 0.0f}, // 5
            {0.5f, 0.5f, 0.0f}, // 6
            {0.0f, 0.0f, 0.0f}, // 4
            {0.0f, 0.5f, 0.0f}, // 7
            {0.0f, 0.0f, 0.0f}, // 8
            {0.0f, 0.0f, 0.0f}, // 9
            {0.0f, 0.0f, 0.0f}, // 10
            {0.0f, 0.0f, 0.0f}  // 11
        };

        if (s_data.batchSize > MAX_BLOCKS)
        {
            DRK_LOG_CORE_INFO("Drawing batch");

            draw();
            flush();
        }

        const uint texIndex = s_data.blockAtlasTextureIndex;

        for (const vec3& vertex : BLOCK_VERTEX_POSITIONS)
        {
            const vec3 vertexPosition = vertex * vec3(position);

            ivec2 subtextureCoordinates = { 0, 0 }; // s, t

            const vec2 texCoord = s_data.blockTextureAtlas->get_coordinates(tid, subtextureCoordinates);

            s_data.vertexBufferData.push_back(
            {
                vertexPosition,
                texCoord,
                texIndex
            });
        }
        s_data.batchSize += 1;

        s_data.stats.blocks += 1;
        s_data.stats.blockFaces += 6;
    }

    const BlockRendererStats& BlockRenderer::get_stats(void)
    {
        return s_data.lastFrameStats;
    }

    void BlockRenderer::reset_stats(void)
    {
        s_data.lastFrameStats = s_data.stats;
        s_data.stats = BlockRendererStats{};
    }

    void BlockRenderer::draw_batch(void)
    {
        DRK_PROFILE_FUNCTION();

        update_vertex_buffer();
        update_index_buffer();

        Renderer::draw_indexed(*s_data.vertexArray);
    }

    void BlockRenderer::flush_batch(void)
    {
        DRK_PROFILE_FUNCTION();

        s_data.vertexBufferData.clear();
        s_data.batchSize = 0;
    }

    void BlockRenderer::update_vertex_buffer(void)
    {
        DRK_PROFILE_FUNCTION();

        s_data.vertexArray->get_vertex_buffer()->update(s_data.vertexBufferData);
    }

    void BlockRenderer::update_index_buffer(void)
    {
        DRK_PROFILE_FUNCTION();

        static constexpr Index BLOCK_INDICES[NUM_INDICES_IN_BLOCK] = {
            0, 1, 3,  1, 2, 3, // Front
            1, 5, 2,  5, 6, 2, // Right
            5, 4, 6,  4, 7, 6, // Back
            4, 0, 7,  0, 3, 7, // Left
            3, 2, 7,  2, 6, 7, // Top
            4, 5, 0,  5, 1, 0  // Bottom
        };

        std::vector<Index> indexBufferData(MAX_BLOCK_INDICES);
        for (uint block = 0; block < s_data.batchSize; ++block)
            for (Index index : BLOCK_INDICES)
                indexBufferData.push_back(index + block * NUM_INDICES_IN_BLOCK);

        s_data.indexBuffer->update(indexBufferData);
    }
}
