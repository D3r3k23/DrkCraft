#ifndef DRK_GRAPHICS_MESH_HPP
#define DRK_GRAPHICS_MESH_HPP

#include "Core/Base.hpp"
#include "Graphics/detail/VertexArray.hpp"

#include "Lib/glm/vec3.hpp"

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

        Mesh(void);

    private:
        VertexArray m_vertexArray;
    };

    class MultiMesh
    {

    };
}

#endif // DRK_GRAPHICS_MESH_HPP
