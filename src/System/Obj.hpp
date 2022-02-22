#ifndef DRK_SYSTEM__OBJ_HPP
#define DRK_SYSTEM__OBJ_HPP

#include "Core/Base.hpp"
#include "Graphics/Mesh.hpp"

#include "lib/fs.hpp"

namespace DrkCraft::Obj
{
    Ptr<MeshData> load_file(const fs::path& filename);
}

#endif // DRK_SYSTEM_OBJ_HPP
