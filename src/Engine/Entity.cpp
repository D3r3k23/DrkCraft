#include "Entity.hpp"

namespace DrkCraft
{
    Entity::Entity(entt::entity handle)
    {

    }

    entt::entity Entity::get_handle(void) const
    {
        return m_handle;
    }
}
