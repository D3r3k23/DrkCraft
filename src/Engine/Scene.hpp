#ifndef DRK_SCENE_HPP
#define DRK_SCENE_HPP

#include "Core/Base.hpp"
#include "Entity.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Camera.hpp"

#include <entt/entt.hpp>
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

        Camera m_camera;

        // std::unordered_map<std::string, ShaderProgram> m_shaderPrograms;
    };
}

#endif // DRK_SCENE_HPP
