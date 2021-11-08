#ifndef DRK_ENTITY_HPP
#define DRK_ENTITY_HPP

#include <entt.hpp>

namespace DrkCraft
{
    class Scene; // Forward declaration, full definition included in Entity.cpp

    class Entity
    {
    public:
        Entity(void) = delete;
        Entity(entt::entity handle, Scene& scene);
        Entity(const Entity& other) = default;

        entt::entity get_handle(void) const;

    private:
        entt::entity m_handle;
        Scene& m_scene;
    };
}

#endif // DRK_ENTITY_HPP
