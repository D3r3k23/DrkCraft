#ifndef DRK_WORLD_HPP
#define DRK_WORLD_HPP

#include "Core/Base.hpp"
#include "ChunkManager.hpp"
#include "ChunkRenderer.hpp"
#include "WorldGenerator.hpp"

#include <entt/entt.hpp>

#include <array>

namespace DrkCraft
{
    class World
    {
    public:

    private:
        std::array<Ref<ChunkManager>, 25> m_loadedChunks;
        std::array<Ref<ChunkRenderer>, 16> m_chunkRenderers;

        entt::registry m_entities;

        WorldGenerator m_generator;
    };
}

#endif // DRK_WORLD_HPP
