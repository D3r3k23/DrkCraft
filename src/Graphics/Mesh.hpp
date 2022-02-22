#ifndef DRK_GRAPHICS_MESH_HPP
#define DRK_GRAPHICS_MESH_HPP

#include "Core/Base.hpp"
#include "Graphics/detail/VertexArray.hpp"

#include "lib/glm/vec3.hpp"

#include <vector>

namespace DrkCraft
{
    struct MeshData
    {

    };

    class Mesh
    {
    public:
        static Ref<Mesh> create(const MeshData& data);

        Mesh();

    private:
        VertexArray m_vertexArray;
    };
}

#endif // DRK_GRAPHICS_MESH_HPP
