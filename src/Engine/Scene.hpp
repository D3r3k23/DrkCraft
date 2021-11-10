#ifndef DRK_SCENE_HPP
#define DRK_SCENE_HPP

#include "Entity.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Shaders.hpp"

#include <entt.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <string_view>
#include <unordered_map>

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

        // std::unordered_map<std::string, ShaderProgram> m_shaderPrograms;
    };
}

#endif // DRK_SCENE_HPP
