#include "GameSystem.hpp"

namespace DrkCraft
{
    GameSystem::GameSystem(World& world, EntityScene& entityScene)
      : m_world(world),
        m_entityScene(entityScene)
    { }
}
