#include "EntityScene.hpp"

#include "Game/Entity/Entity.hpp"

namespace DrkCraft::Game
{
    EntityScene::EntityScene(void)
    {

    }

    Entity EntityScene::create(void)
    {
        return Entity(m_registry);
    }

    void EntityScene::destroy(Entity entity)
    {
        entity.destroy();
    }
}
