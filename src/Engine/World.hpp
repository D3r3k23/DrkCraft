#ifndef DRK_WORLD_HPP
#define DRK_WORLD_HPP

#include "Core/Base.hpp"
#include "ChunkManager.hpp"
#include "WorldGenerator.hpp"
#include "Noise.hpp"
#include "Core/Timestep.hpp"
#include "Graphics/Camera.hpp"

#include <entt/entt.hpp>

namespace DrkCraft
{
    class World
    {
    public:
        World(void);
        ~World(void);

        void update(Timestep timestep);
        void render(void);

    private:
        void render_chunks(void);
        void render_entities(void);

    private:
        ChunkManager m_chunkManager;

        entt::registry m_entities; // EntityManager

        Noise m_noiseMap;
        WorldGenerator m_generator;
    };
}

#endif // DRK_WORLD_HPP
