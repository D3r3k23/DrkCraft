#ifndef DRK_GAME_SYSTEMS_PHYSICS_HPP
#define DRK_GAME_SYSTEMS_PHYSICS_HPP

#include "Core/Base.hpp"
#include "Game/GameSystem.hpp"
#include "Util/Timestep.hpp"
#include "Game/World/World.hpp"
#include "Game/Entity/Scene.hpp"

#include "Game/World/World.hpp"
#include "Game/Player.hpp"

namespace DrkCraft::Game
{
    class PhysicsSystem : public GameSystem
    {
    public:
        PhysicsSystem(const GameSystemData& data);
        virtual ~PhysicsSystem(void) = default;

        virtual void render(void) override;
        virtual void update(Timestep timestep) override;
    };
}

#endif // DRK_GAME_SYSTEMS_PHYSICS_HPP
