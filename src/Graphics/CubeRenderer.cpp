#include "CubeRenderer.hpp"

#include "Renderer.hpp"
#include "Shader.hpp"
#include "System/AssetManager.hpp"
#include "Core/Profiler.hpp"

#include "Util.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
    static constexpr float CUBE_SIZE = 0.5f;

    static constexpr uint MAX_CUBES = 1000;

    static constexpr uint NUM_VERTICES_IN_CUBE = 8;
    static constexpr uint NUM_INDICES_IN_CUBE  = 8 * 6;

    static constexpr uint MAX_CUBE_VERTICES = MAX_CUBES * NUM_VERTICES_IN_CUBE;
    static constexpr uint MAX_CUBE_INDICES  = MAX_CUBES * NUM_INDICES_IN_CUBE;

    struct CubeVertex
    {
        glm::vec3 position;
        glm::vec2 texCoord;
        float     texIndex;
    };

    struct CubeRendererData
    {
        CubeRendererStats stats;

        ShaderProgram cubeShader{"CubeShader"};

        Ptr<VertexArray> cubeVertexArray;

        Ref<VertexBuffer> cubeVertexBuffer;
        Ref<IndexBuffer>  cubeIndexBuffer;

        std::vector<CubeVertex> cubeVertexBufferData;

        uint nCubeIndices = 0;
    };

    static CubeRendererData s_data;

    void CubeRenderer::init(void)
    {
        DRK_PROFILE_FUNCTION();

        std::vector<Ref<Shader>> shaders = {
          Shader::create(shader_asset_path("cube_vertex_shader.glsl"), ShaderType::Vertex),
          Shader::create(shader_asset_path("cube_fragment_shader.glsl"), ShaderType::Fragment)
        };
        s_data.cubeShader.attach(shaders);
        s_data.cubeShader.link();

        s_data.cubeVertexArray  = make_ptr<VertexArray>();
        s_data.cubeVertexBuffer = make_ref<VertexBuffer>(MAX_CUBE_VERTICES);
        s_data.cubeIndexBuffer  = make_ref<IndexBuffer>(MAX_CUBE_INDICES);

        s_data.cubeVertexBufferData.resize(MAX_CUBE_VERTICES);

        s_data.cubeVertexBuffer->set_layout({
            { ShaderDataType::Float3, "a_position" },
            { ShaderDataType::Float2, "a_texCoord" },
            { ShaderDataType::Float,  "a_texIndex" }
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

        s_data.cubeIndexBuffer->set_data(cubeIndexBufferData);

        s_data.cubeVertexArray->set_vertex_buffer(s_data.cubeVertexBuffer);
        s_data.cubeVertexArray->set_index_buffer(s_data.cubeIndexBuffer);



        // Triangle strip
        // glm::vec3 vertices[NUM_VERTICES] = {
        //     {0.5f, 0.5f, 0.0f},
        //     {0.0f, 0.5f, 0.0f},
        //     {0.5f, 0.5f, 0.5f},
        //     {0.0f, 0.5f, 0.5f},
        //     {0.5f, 0.0f, 0.0f},
        //     {0.0f, 0.0f, 0.0f},
        //     {0.0f, 0.0f, 0.5f},
        //     {0.5f, 0.0f, 0.5f}
        // };
        // Index indices[NUM_INDICES] = { 3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0 };

        glm::vec3 vertices[] = {
            {0.0f, 0.0f, 0.5f}, // 0
            {0.5f, 0.0f, 0.5f}, // 1
            {0.5f, 0.5f, 0.5f}, // 2
            {0.0f, 0.5f, 0.5f}, // 3
            {0.5f, 0.0f, 0.0f}, // 5
            {0.5f, 0.5f, 0.0f}, // 6
            {0.0f, 0.0f, 0.0f}, // 4
            {0.0f, 0.5f, 0.0f}  // 7
        };
        Index indices[] = {
            0, 1, 3,  1, 2, 3, // Front
            1, 5, 2,  5, 6, 2, // Right
            5, 4, 6,  4, 7, 6, // Back
            4, 0, 7,  0, 3, 7, // Left
            3, 2, 7,  2, 6, 7, // Top
            4, 5, 0,  5, 1, 0  // Bottom
        };

        GlObjectHandler<VertexBuffer> vbo(*cubeVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Bind TextureShaderProgram? (if bool textures == true)
    }

    void CubeRenderer::shutdown(void)
    {
        s_data.cubeVertexArray.reset();
        s_data.cubeVertexBuffer.reset();
    }

    void CubeRenderer::begin_scene(void)
    {
        const Camera& camera = Renderer::get_camera();
        s_data.cubeShader.bind();
        s_data.cubeShader.upload_uniform("u_viewProjection", camera.get_projection_matrix());
    }

    void CubeRenderer::end_scene(void)
    {
        flush();
    }

    void CubeRenderer::submit(const glm::ivec3& position, const Ref<Texture2D>& texture, const glm::vec4& color)
    {
        DRK_PROFILE_FUNCTION();

        static constexpr glm::vec3 cubeVertexPositions[NUM_VERTICES_IN_CUBE] = {
            {0.0f, 0.0f, 0.5f}, // 0
            {0.5f, 0.0f, 0.5f}, // 1
            {0.5f, 0.5f, 0.5f}, // 2
            {0.0f, 0.5f, 0.5f}, // 3
            {0.5f, 0.0f, 0.0f}, // 5
            {0.5f, 0.5f, 0.0f}, // 6
            {0.0f, 0.0f, 0.0f}, // 4
            {0.0f, 0.5f, 0.0f}  // 7
        };

        glm::vec2 texCoord;
        uint texIndex;

        for (const auto& cubeVertex : cubeVertexPositions)
        {
            const glm::vec3 vertexPosition = cubeVertex * glm::vec3(position);
            s_data.cubeVertexBufferData.push_back({
                vertexPosition,
                texCoord,
                texIndex});
        }

        s_data.stats.cubes++;
        s_data.stats.triangles += 12;
        s_data.stats.vertices  += 8;
        s_data.stats.indices   += 36;
    }

    void CubeRenderer::submit(const glm::ivec3& position, const Ref<Texture2D>& texture)
    {
        submit(position, texture, s_data.whiteColor);
    }

    void CubeRenderer::submit(const glm::ivec3& position, const glm::vec4& color)
    {
        submit(position, s_data.whiteTexture, color);
    }

    const CubeRendererStats& CubeRenderer::get_stats(void)
    {
        return s_data.stats;
    }

    void CubeRenderer::reset_stats(void)
    {
        s_data.stats = RendererStats{};
    }

    void CubeRenderer::start_batch(void)
    {
        s_data.nCubeIndices = 0;
    }

    void CubeRenderer::next_batch(void)
    {
        flush();
        start_batch();
    }

    void CubeRenderer::flush(void)
    {
        DRK_PROFILE_FUNCTION();

        m_vertexBuffer->set_data(m_vertexBufferData);
        Renderer::draw(m_vertexArray);
    }




    void CubeRenderer::draw_cube(const Transform& transform)
    {
        GlObjectHandler<VertexBuffer> vbo(*m_cubeVertexBuffer);
        glEnableVertexAttribArray(0);
        GlObjectHandler<IndexBuffer> vao(*m_cubeIndexBuffer);
        Renderer::draw_indexed(*m_cubeIndexBuffer);
        glDisableVertexAttribArray(0);
    }
}
