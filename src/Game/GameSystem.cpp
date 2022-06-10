#include "GameSystem.hpp"

namespace DrkCraft::Game
{
    GameSystem::GameSystem(World& world, EntityScene& entityScene)
      : m_world(world),
        m_entityScene(entityScene)
    { }
}
