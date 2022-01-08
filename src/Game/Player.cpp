#include "Player.hpp"

#include "Application/Input.hpp"
#include "Core/Profiler.hpp"

#include <glm/geometric.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace DrkCraft
{
    Player::Player(void)
      : m_camera(45.0f, 16.9f, 0.01f, 1000.0f), //
        m_position(0.0f, 0.0f, -2.0f),
        m_direction(0.0f, 0.0f, 1.0f),
        m_viewpointOffset(0.0f, 0.0f, 0.0f), // 2.0f?
        m_flying(true), // false
        m_speed(1.0f)
    { }

    void Player::on_update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        const float ts{timestep};
        const glm::vec3 horizontalDirection = {m_direction.x, m_direction.y, 0.0};

        if (is_key_pressed(KeyCode::W)) m_position += m_speed * ts * horizontalDirection;
        if (is_key_pressed(KeyCode::S)) m_position -= m_speed * ts * horizontalDirection;
        if (is_key_pressed(KeyCode::A)) m_position -= m_speed * ts * glm::normalize(glm::cross(horizontalDirection, {0.0f, 1.0f, 0.0f}));
        if (is_key_pressed(KeyCode::D)) m_position += m_speed * ts * glm::normalize(glm::cross(horizontalDirection, {0.0f, 1.0f, 0.0f}));

        if (is_key_pressed(KeyCode::Space))
        {
            if (m_flying)
            {
                m_position.y += m_speed * ts;
            }
            else // Jumping should be handled by events
            {

            }
        }

        if (is_mod_pressed(KeyMod::Ctrl))
        {
            if (m_flying)
            {
                m_position.y -= m_speed * ts;
            }
            else
            {

            }
        }
    }

    glm::mat4 Player::get_view_projection(void) const
    {
        const glm::vec3 viewpoint{m_position + m_viewpointOffset};
        return glm::lookAt(viewpoint, viewpoint + m_direction, {0.0f, 1.0f, 0.0f});
    }
}
