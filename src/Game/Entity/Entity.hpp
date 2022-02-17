#ifndef DRK_GAME_ENTITY_ENTITY_HPP
#define DRK_GAME_ENTITY_ENTITY_HPP

#include <entt/entt.hpp>

namespace DrkCraft
{
    class Scene; // Forward declaration, full definition included in Entity.cpp

    class Entity
    {
    public:
        Entity(void) = delete;
        Entity(entt::entity handle);
        Entity(const Entity& other) = default;

        entt::entity get_handle(void) const;

    private:
        entt::entity m_handle;
    };
}

#endif // DRK_GAME_ENTITY_ENTITY_HPP
