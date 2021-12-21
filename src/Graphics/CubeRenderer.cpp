#include "CubeRenderer.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace DrkCraft
{
    const int NUM_VERTICES = 8;
    const int NUM_INDICES  = 14;

    CubeRenderer::CubeRenderer(void)
    {
        glm::vec3 vertices[NUM_VERTICES] = {
            {0.5f, 0.5f, 0.0f},
            {0.0f, 0.5f, 0.0f},
            {0.5f, 0.5f, 0.5f},
            {0.0f, 0.5f, 0.5f},
            {0.5f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.5f},
            {0.5f, 0.0f, 0.5f}
        };
        Index indices[NUM_INDICES] = { 3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0 };

        cubeVertexBuffer = make_ptr<VertexBuffer>(glm::value_ptr(vertices[0]), NUM_VERTICES);
        cubeIndexBuffer  = make_ptr<IndexBuffer>(indices, NUM_INDICES);
    }

    void CubeRenderer::draw_cube(const Transform& transform)
    {
        cubeVertexBuffer->bind();
        cubeIndexBuffer->bind();
        glDrawElements(GL_TRIANGLE_STRIP, NUM_INDICES, GL_UNSIGNED_INT, nullptr);
        cubeVertexBuffer->unbind();
        cubeIndexBuffer->unbind();
    }
}
