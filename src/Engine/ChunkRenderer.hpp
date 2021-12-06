#ifndef DRK_CHUNK_RENDERER_HPP
#define DRK_CHUNK_RENDERER_HPP // ChunkMesher?

#include "Core/Base.hpp"
#include "Chunk.hpp"
#include "Graphics/Mesh.hpp"

namespace DrkCraft
{
    class ChunkRenderer
    {
    public:
        ChunkRenderer(const Chunk& chunk);

        void render(void) const;

    private:
        // CubeMesh m_mesh;
    };
}

#endif // DRK_CHUNK_RENDERER_HPP
