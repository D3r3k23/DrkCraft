#include "Mesh.hpp"

#include "Disk/Obj.hpp"

namespace DrkCraft
{
    Ref<Mesh> Mesh::create(const MeshData& data)
    {
        return make_ptr<Mesh>();
    }

    Mesh::Mesh(void)
    {

    }
}
