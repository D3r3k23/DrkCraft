#include "Physics.hpp"

#include "Core/Debug/Profiler.hpp"

namespace DrkCraft::Game
{
    PhysicsSystem::PhysicsSystem(World& world, EntityScene& entityScene)
      : GameSystem(world, entityScene)
    { }

    void PhysicsSystem::render(void)
    {
        DRK_PROFILE_FUNCTION();


    }

    void PhysicsSystem::update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();


    }
}

