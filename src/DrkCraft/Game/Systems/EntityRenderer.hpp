#ifndef DRK_GAME_SYSTEMS_ENTITY_RENDERER_HPP
#define DRK_GAME_SYSTEMS_ENTITY_RENDERER_HPP

#include "Core/Base.hpp"
#include "Game/GameSystem.hpp"
#include "Util/Timestep.hpp"
#include "Game/World/World.hpp"
#include "Game/Entity/Scene.hpp"

namespace DrkCraft::Game
{
    class EntityRendererSystem : public GameSystem
    {
    public:
        EntityRendererSystem(const GameSystemData& data);
        virtual ~EntityRendererSystem(void) = default;

        virtual void render(void) override;
        virtual void update(Timestep timestep) override;
    };
}

#endif // DRK_GAME_SYSTEMS_ENTITY_RENDERER_HPP
