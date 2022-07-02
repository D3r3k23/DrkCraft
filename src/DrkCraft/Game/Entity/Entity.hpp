#ifndef DRK_GAME_ENTITY_ENTITY_HPP
#define DRK_GAME_ENTITY_ENTITY_HPP

#include "Core/Base.hpp"

#include <entt/entity/registry.hpp>

namespace DrkCraft::Game
{
    using EntityID = uint32; // 16 <--

    class Entity
    {
        friend class Scene;

    private:
        Entity(entt::registry& registry)
          : m_registry(registry),
            m_handle(m_registry.create())
        { }

        void destroy(void)
        {
            m_registry.destroy(m_handle);
            m_handle = entt::null;
        }

    public:
        Entity(const Entity&) = default;
        Entity& operator=(const Entity&) = default;

        operator bool(void) const
        {
            return m_handle != entt::null;
        }

        template <typename T, typename ... Args>
        T& add_component(Args&& ... args)
        {
            return m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
        }

        template <typename T>
        bool has_component(void)
        {
            return m_registry.all_of<T>(m_handle);
        }

        template <typename ... Args>
        bool has_all_components(void) const
        {
            return m_registry.all_of<Args>(m_handle);
        }

        template <typename ... Args>
        bool has_any_component(void) const
        {
            return m_registry.any_of<Args>(m_handle);
        }

        template <typename T>
        T& get_component(void) const
        {
            return m_registry.get<T>(m_handle);
        }

        template <typename T>
        void remove_component(void)
        {
            m_registry.remove<T>(m_handle);
        }

    private:
        entt::registry& m_registry;
        entt::entity m_handle;
    };
}

#endif // DRK_GAME_ENTITY_ENTITY_HPP
