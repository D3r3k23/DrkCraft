#include "CubeRenderer.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace DrkCraft
{
    static const int NUM_VERTICES = 8;
    static const int NUM_INDICES  = 36;

    CubeRenderer::CubeRenderer(void)
    {
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

        glm::vec3 vertices[NUM_VERTICES] = {
            {0.0f, 0.0f, 0.5f}, // 0
            {0.5f, 0.0f, 0.5f}, // 1
            {0.5f, 0.5f, 0.5f}, // 2
            {0.0f, 0.5f, 0.5f}, // 3
            {0.5f, 0.0f, 0.0f}, // 5
            {0.5f, 0.5f, 0.0f}, // 6
            {0.0f, 0.0f, 0.0f}, // 4
            {0.0f, 0.5f, 0.0f}  // 7
        };
        Index indices[NUM_INDICES] = {
            0, 1, 3,  1, 2, 3, // Front
            1, 5, 2,  5, 6, 2, // Right
            5, 4, 6,  4, 7, 6, // Back
            4, 0, 7,  0, 3, 7, // Left
            3, 2, 7,  2, 6, 7, // Top
            4, 5, 0,  5, 1, 0  // Bottom
        };

        cubeVertexBuffer = make_ptr<VertexBuffer>(glm::value_ptr(vertices[0]), NUM_VERTICES);
        cubeIndexBuffer  = make_ptr<IndexBuffer>(indices, NUM_INDICES);
    }

    void CubeRenderer::draw_cube(const Transform& transform)
    {
        cubeVertexBuffer->bind();
        cubeIndexBuffer->bind();
        glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, nullptr);
        cubeVertexBuffer->unbind();
        cubeIndexBuffer->unbind();
    }
}
