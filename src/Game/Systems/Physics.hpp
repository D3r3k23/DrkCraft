#ifndef DRK_GAME_SYSTEMS_PHYSICS_HPP
#define DRK_GAME_SYSTEMS_PHYSICS_HPP

#include "Core/Base.hpp"
#include "Game/System.hpp"
#include "Application/Timestep.hpp"
#include "Game/Entity/EntityScene.hpp"

#include "Game/World/World.hpp"
#include "Game/Player.hpp"

namespace DrkCraft
{
    class PhysicsSystem : public System
    {
    public:
        PhysicsSystem(EntityScene& scene, World& world, Player& player);
        ~PhysicsSystem(void) = default;

        virtual void update(Timestep timestep) override;

    private:
        World& m_world;
        Player& m_player;
    };
}

#endif // DRK_GAME_SYSTEMS_PHYSICS_HPP
