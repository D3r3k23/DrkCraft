#ifndef DRK_ENTITY_ENTITY_SCENE_HPP
#define DRK_ENTITY_ENTITY_SCENE_HPP

#include "Core/Base.hpp"

#include <entt/entity/registry.hpp>

namespace DrkCraft
{
    class EntityScene
    {
    public:
        EntityScene(void);

    private:
        entt::registry m_registry;
    };
}

#endif // DRK_ENTITY_ENTITY_SCENE_HPP
