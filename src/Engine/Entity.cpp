#include "Entity.hpp"

#include "Scene.hpp"

namespace DrkCraft
{
    Entity::Entity(entt::entity handle, Scene& scene)
      : m_scene(scene)
    {

    }

    entt::entity Entity::get_handle(void) const
    {
        return m_handle;
    }
}
