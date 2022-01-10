#ifndef DRK_PLAYER_HPP
#define DRK_PLAYER_HPP

#include "Core/Base.hpp"
#include "Graphics/Camera.hpp"
#include "Application/Timestep.hpp"

#include <glm/vec3.hpp>

namespace DrkCraft
{
    enum class PlayerState
    {
        Standing,
        Jumping,
        Crouching
    };

    class Player
    {
    public:
        Player(void);

        void on_update(Timestep timestep);

        glm::mat4 get_view_projection(void) const;

    private:
        glm::vec3 m_position;
        glm::vec3 m_direction;
        glm::vec3 m_viewpointOffset;

        bool m_flying;

        float m_speed;

        Camera m_camera;
    };
}

#endif // DRK_PLAYER_HPP
