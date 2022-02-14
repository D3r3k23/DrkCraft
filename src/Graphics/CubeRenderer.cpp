#include "CubeRenderer.hpp"

#include "Graphics/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/detail/VertexArray.hpp"
#include "Graphics/detail/Buffer.hpp"
#include "Graphics/detail/Util.hpp"
#include "System/AssetManager.hpp"
#include "Core/Debug/Profiler.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
    static constexpr float CUBE_SIZE = 0.5f;

    static constexpr uint MAX_CUBES = 1000;

    static constexpr uint NUM_VERTICES_IN_CUBE = 8;
    static constexpr uint NUM_INDICES_IN_CUBE  = 8 * 6;

    static constexpr uint MAX_CUBE_VERTICES = MAX_CUBES * NUM_VERTICES_IN_CUBE;
    static constexpr uint MAX_CUBE_INDICES  = MAX_CUBES * NUM_INDICES_IN_CUBE;

    namespace
    {
        struct CubeVertex
        {
            vec3 position;
            vec2 texCoord;
            uint texIndex;
        };

        struct CubeRendererData
        {
            CubeRendererStats lastStats;
            CubeRendererStats stats;

            ShaderProgram shader{"CubeShader"};

            Ptr<VertexArray> vertexArray;

            Ref<VertexBuffer> vertexBuffer;
            Ref<IndexBuffer>  indexBuffer;

            std::vector<CubeVertex> vertexBufferData;

            uint indices = 0;

            vec4 whiteColor;
            Ref<Texture2D> whiteTexture;
        };
    }

    static CubeRendererData s_data;

    void CubeRenderer::init(void)
    {
        DRK_PROFILE_FUNCTION();

        std::vector<Ref<Shader>> shaders = {
          Shader::create(shader_asset_path("cube_vertex_shader.glsl"), ShaderType::Vertex),
          Shader::create(shader_asset_path("cube_fragment_shader.glsl"), ShaderType::Fragment)
        };
        s_data.shader.attach(shaders);
        s_data.shader.link();

        const uint vbSize = MAX_CUBE_VERTICES * sizeof(CubeVertex);
        const uint ibSize = MAX_CUBE_VERTICES * sizeof(IndexBuffer);
        s_data.vertexArray  = make_ptr<VertexArray>();
        s_data.vertexBuffer = make_ref<VertexBuffer>(vbSize, PrimitiveType::Triangles);
        s_data.indexBuffer  = make_ref<IndexBuffer>(ibSize);

        s_data.vertexBufferData.resize(MAX_CUBE_VERTICES);

        s_data.vertexBuffer->set_layout({
            { ShaderDataType::Float3, "a_position" },
            { ShaderDataType::Float2, "a_texCoord" },
            { ShaderDataType::Uint,   "a_texIndex" }
        });

        Index cubeIndices[NUM_INDICES_IN_CUBE] = {
            0, 1, 3,  1, 2, 3, // Front
            1, 5, 2,  5, 6, 2, // Right
            5, 4, 6,  4, 7, 6, // Back
            4, 0, 7,  0, 3, 7, // Left
            3, 2, 7,  2, 6, 7, // Top
            4, 5, 0,  5, 1, 0  // Bottom
        };

        std::vector<Index> cubeIndexBufferData(MAX_CUBE_INDICES);
        for (uint c = 0; c < MAX_CUBES; c++)
            for (uint i = 0; i < NUM_INDICES_IN_CUBE; i++)
                cubeIndexBufferData.push_back(cubeIndices[i]);

        s_data.indexBuffer->update(cubeIndexBufferData);

        s_data.vertexArray->set_vertex_buffer(s_data.vertexBuffer);
        s_data.vertexArray->set_index_buffer(s_data.indexBuffer);
    }

    void CubeRenderer::shutdown(void)
    {
        s_data.vertexArray.reset();
        s_data.vertexBuffer.reset();
    }

    void CubeRenderer::begin_scene(void)
    {
        const Camera& camera = Renderer::get_camera();
        s_data.shader.bind();
        s_data.shader.upload_uniform("u_viewProjection", camera.get_view_projection());

        reset_stats();

        s_data.vertexArray->bind();
        start_batch();
    }

    void CubeRenderer::end_scene(void)
    {
        flush();

        s_data.vertexArray->unbind();
    }

    void CubeRenderer::submit(const ivec3& position, const Ref<Texture2D>& texture, const vec4& color)
    {
        DRK_PROFILE_FUNCTION();

        static constexpr vec3 cubeVertexPositions[NUM_VERTICES_IN_CUBE] = {
            {0.0f, 0.0f, 0.5f}, // 0
            {0.5f, 0.0f, 0.5f}, // 1
            {0.5f, 0.5f, 0.5f}, // 2
            {0.0f, 0.5f, 0.5f}, // 3
            {0.5f, 0.0f, 0.0f}, // 5
            {0.5f, 0.5f, 0.0f}, // 6
            {0.0f, 0.0f, 0.0f}, // 4
            {0.0f, 0.5f, 0.0f}  // 7
        };

        vec2 texCoord;
        uint texIndex;

        for (const auto& cubeVertex : cubeVertexPositions)
        {
            const vec3 vertexPosition = cubeVertex * vec3(position);
            s_data.vertexBufferData.push_back({
                vertexPosition,
                texCoord,
                texIndex
            });
        }

        s_data.stats.cubes++;
        s_data.stats.cubeFaces += 6;
    }

    void CubeRenderer::submit(const ivec3& position, const Ref<Texture2D>& texture)
    {
        submit(position, texture, s_data.whiteColor);
    }

    void CubeRenderer::submit(const ivec3& position, const vec4& color)
    {
        submit(position, s_data.whiteTexture, color);
    }

    const CubeRendererStats& CubeRenderer::get_stats(void)
    {
        return s_data.lastStats;
    }

    void CubeRenderer::reset_stats(void)
    {
        s_data.lastStats = s_data.stats;
        s_data.stats = CubeRendererStats{};
    }

    void CubeRenderer::start_batch(void)
    {
        s_data.indices = 0;
    }

    void CubeRenderer::next_batch(void)
    {
        flush();
        start_batch();
    }

    void CubeRenderer::flush(void)
    {
        DRK_PROFILE_FUNCTION();

        s_data.vertexArray->get_vertex_buffer()->update<CubeVertex>(s_data.vertexBufferData);
        Renderer::draw_indexed(s_data.vertexArray->get_index_buffer(), s_data.indices);
    }
}
