#ifndef DRK_UTIL_OBJ_HPP
#define DRK_UTIL_OBJ_HPP

#include "Core/Base.hpp"
#include "Graphics/Mesh.hpp"

#include "lib/fs.hpp"

namespace DrkCraft::Obj
{
    Ptr<MeshData> load_file(const fs::path& filename);
}

#endif // DRK_UTIL_OBJ_HPP
