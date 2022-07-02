#include "Scene.hpp"

#include "Game/Entity/Entity.hpp"

namespace DrkCraft::Game
{
    Scene::Scene(void)
    {

    }

    Entity Scene::create(void)
    {
        return Entity(m_registry);
    }

    void Scene::destroy(Entity entity)
    {
        entity.destroy();
    }
}
