#include "EntityManager.hpp"

#include "Game/Entity/Entity.hpp"

namespace DrkCraft::Game
{
    EntityManager::EntityManager(void)
    {

    }

    Entity EntityManager::create(void)
    {
        return Entity(m_registry);
    }

    void EntityManager::destroy(Entity entity)
    {
        entity.destroy();
    }
}
