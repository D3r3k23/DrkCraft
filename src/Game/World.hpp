#ifndef DRK_GAME_WORLD_HPP
#define DRK_GAME_WORLD_HPP

#include "Core/Base.hpp"
#include "Game/ChunkManager.hpp"
#include "Game/WorldGenerator.hpp"
#include "Game/Player.hpp"
#include "Application/Timestep.hpp"
#include "Util/Noise.hpp"

#include <entt/entt.hpp>

#include "lib/fs.hpp"

namespace DrkCraft
{
    struct World
    {
    public:
        static Ptr<World> load_save(const fs::path& saveDir);

        void render(void);

        void render_chunks(void);
        void render_entities(void);

        Player player;

        ChunkManager chunkManager;

        entt::registry entities; // EntityManager

        Noise noiseMap;
        WorldGenerator generator;
    };
}

#endif // DRK_GAME_WORLD_HPP
