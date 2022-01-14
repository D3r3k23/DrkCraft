#include "CubeRenderer.hpp"

#include "Renderer.hpp"
#include "Core/Profiler.hpp"

#include "Util.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
    CubeRenderer::CubeRenderer(void)
    {
        DRK_PROFILE_FUNCTION();

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

        m_cubeVertexBuffer = make_ptr<VertexBuffer>(vertices);
        m_cubeIndexBuffer = make_ptr<IndexBuffer>(indices);

        GlObjectHandler<VertexBuffer> vbo(*m_cubeVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
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
