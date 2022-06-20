#ifndef DRK_GAME_SYSTEMS_WORLD_RENDERER_HPP
#define DRK_GAME_SYSTEMS_WORLD_RENDERER_HPP

#include "Core/Base.hpp"
#include "Game/GameSystem.hpp"
#include "Util/Timestep.hpp"
#include "Game/World/World.hpp"
#include "Game/Entity/EntityManager.hpp"

namespace DrkCraft::Game
{
    class WorldRendererSystem : public GameSystem
    {
    public:
        WorldRendererSystem(const GameSystemData& data);
        virtual ~WorldRendererSystem(void) = default;

        virtual void render(void) override;
        virtual void update(Timestep timestep) override;

    private:
        void render_chunk(const Chunk& chunk);

    private:
    };
}

#endif // DRK_GAME_SYSTEMS_WORLD_RENDERER_HPP
