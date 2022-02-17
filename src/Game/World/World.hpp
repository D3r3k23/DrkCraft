#ifndef DRK_GAME_WORLD_WORLD_HPP
#define DRK_GAME_WORLD_WORLD_HPP

#include "Core/Base.hpp"
#include "Game/World/ChunkManager.hpp"
#include "Game/World/WorldGenerator.hpp"
#include "Application/Timestep.hpp"
#include "Util/Noise.hpp"

#include <entt/entt.hpp>

#include "lib/fs.hpp"

namespace DrkCraft
{
    struct World
    {
    public:
        World(void) = default;

        void render(void);

        void render_chunks(void);
        void render_entities(void);

        ChunkManager chunkManager;

        entt::registry entities; // EntityManager

        Noise noiseMap;
        WorldGenerator generator;
    };
}

#endif // DRK_GAME_WORLD_WORLD_HPP
