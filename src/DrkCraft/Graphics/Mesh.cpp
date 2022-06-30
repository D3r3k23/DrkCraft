#include "Mesh.hpp"

#include "Util/Obj.hpp"

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
