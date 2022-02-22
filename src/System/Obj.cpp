#include "Obj.hpp"

#include "Core/Debug/Profiler.hpp"

#include <tiny_obj_loader.h>

namespace DrkCraft::Obj
{
    Ptr<MeshData> load_file(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Reading .obj file \"{}\"", filename.generic_string());

        tinyobj::ObjReaderConfig config;

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(filename.string(), config))
        {
            if (!reader.Error().empty())
                DRK_LOG_CORE_ERROR("tinyobjloader error: {}", reader.Error());
            DRK_ASSERT_CORE(false, "Failed to read .obj file: {}", filename.generic_string());
        }
    #if defined(DRK_CONFIG_DEBUG)
        if (!reader.Warning().empty())
            DRK_LOG_CORE_WARN("tinyobjloader error: {}", reader.Error());
    #endif
        const auto& shapes  = reader.GetShapes();
        const auto& vertices = reader.GetAttrib().vertices;
        const auto& normals   = reader.GetAttrib().normals;
        const auto& texCoords  = reader.GetAttrib().texcoords;

        for (const auto& shape : shapes) // 1?
        {
            uint indexOffset = 0;
            const auto& mesh = shape.mesh;
            const auto& vFaceVertices = mesh.num_face_vertices;

            for (const auto& nVertices : vFaceVertices)
            {
                for (uint v = 0; v < nVertices; v++)
                {
                    const auto index = mesh.indices[indexOffset + v];
                    const auto vx = vertices[3 * index.vertex_index];
                    const auto vy = vertices[3 * index.vertex_index + 1];
                    const auto vz = vertices[3 * index.vertex_index + 2];

                    const auto nx = normals[3 * index.normal_index];
                    const auto ny = normals[3 * index.normal_index + 1];
                    const auto nz = normals[3 * index.normal_index + 2];

                    const auto tx = vertices[2 * index.texcoord_index];
                    const auto ty = vertices[2 * index.texcoord_index + 1];
                }
                indexOffset += nVertices;
            }
        }
        return {};
    }
}
