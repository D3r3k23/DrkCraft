#ifndef DRK_SCENE_HPP
#define DRK_SCENE_HPP

#include "Entity.hpp"

#include <entt.hpp>
#include <glm/mat4x4.hpp>

#include <string_view>

namespace DrkCraft
{
    class Scene
    {
    public:
        Scene(void) = default;
        ~Scene(void) = default;

        Entity add_entity(std::string_view name);

    private:
        entt::registry m_registry;

        glm::mat4 viewProjection;
    };
}

#endif // DRK_SCENE_HPP
