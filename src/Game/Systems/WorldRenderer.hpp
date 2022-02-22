#ifndef DRK_GAME_SYSTEMS_WORLD_RENDERER_HPP
#define DRK_GAME_SYSTEMS_WORLD_RENDERER_HPP

#include "Core/Base.hpp"
#include "Game/GameSystem.hpp"
#include "Core/Timestep.hpp"
#include "Game/World/World.hpp"
#include "Game/Entity/EntityScene.hpp"

namespace DrkCraft
{
    class WorldRendererSystem : public GameSystem
    {
    public:
        WorldRendererSystem(World& world, EntityScene& entityScene);
        virtual ~WorldRendererSystem(void) = default;

        virtual void render(void) override;
        virtual void update(Timestep timestep) override;

    private:
        void render_chunk(const Chunk& chunk);

    private:
    };
}

#endif // DRK_GAME_SYSTEMS_WORLD_RENDERER_HPP
