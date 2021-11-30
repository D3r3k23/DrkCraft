#ifndef DRK_MESH_HPP
#define DRK_MESH_HPP

#include "Core/Base.hpp"
#include "Buffer.hpp"

#include <glm/vec3.hpp>

#include <vector>
#include <span>

namespace DrkCraft
{
    class CubeMesh
    {
    public:
        CubeMesh(std::span<glm::vec3> vertices);

        std::vector<glm::vec3> get_vertices(void) const;

    private:
        VertexBuffer m_vertexBuffer;
    };
}

#endif // DRK_MESH_HPP
