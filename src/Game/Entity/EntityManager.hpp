#ifndef DRK_ENTITY_ENTITY_MANAGER_HPP
#define DRK_ENTITY_ENTITY_MANAGER_HPP

#include "Core/Base.hpp"

#include <entt/entity/registry.hpp>

namespace DrkCraft::Game
{
    class Entity;

    class EntityManager
    {
    public:
        EntityManager(void);

        Entity create(void);
        void destroy(Entity entity);

        template <typename ... Args>
        auto view(void)
        {
            return m_registry.view<Args...>();
        }

        template <typename ... Args>
        const auto view(void) const
        {
            return m_registry.view<Args...>();
        }

    private:
        entt::registry m_registry;
    };
}

#endif // DRK_ENTITY_ENTITY_MANAGER_HPP