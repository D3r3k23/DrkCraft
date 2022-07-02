#ifndef DRK_DISK_OBJ_HPP
#define DRK_DISK_OBJ_HPP

#include "Core/Base.hpp"
#include "Graphics/Mesh.hpp"

#include "Lib/fs.hpp"

namespace DrkCraft::Obj
{
    Ptr<MeshData> load_file(const fs::path& filename);
}

#endif // DRK_DISK_OBJ_HPP
