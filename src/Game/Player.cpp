#include "Player.hpp"

#include "System/Input.hpp"
#include "Core/RunSettings.hpp"
#include "Core/Debug/Profiler.hpp"

#include <glm/geometric.hpp>

namespace DrkCraft
{
    static constexpr float FLYING_SPEED = 1.0f;

    Player::Player(void)
      : m_position(0.0f, 0.0f, -2.0f),
        m_direction(0.0f, 0.0f, 1.0f),
        m_state(PlayerState::Normal),
        m_distanceAboveGround(0.0f),
        m_camera(45.0f, 16.9f, 0.01f, 1000.0f, m_position, m_direction),
        m_cameraOffset(0.0f, 2.0f, 0.0f)
    { }

    void Player::update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        const float delta{timestep};
        const vec3 horizontalDirection = {m_direction.x, m_direction.y, 0.0};

        float speed = 1.0f;

        switch (m_state)
        {
            case PlayerState::Normal:
            {
                if (is_mod_pressed(KeyMod::Shift))
                {
                    m_state = PlayerState::Sprinting;
                }
                else if (is_mod_pressed(KeyMod::Ctrl))
                {
                    m_state = PlayerState::Crouching;
                }
                break;
            }
            case PlayerState::Sprinting:
            {
                if (!is_mod_pressed(KeyMod::Shift))
                {
                    m_state = PlayerState::Normal;
                }
                else if (is_mod_pressed(KeyMod::Ctrl))
                {
                    m_state = PlayerState::Crouching;
                }
                speed *= 1.5;
                break;
            }
            case PlayerState::Crouching:
            {
                if (!is_mod_pressed(KeyMod::Ctrl))
                {
                    m_state = PlayerState::Normal;
                }
                speed *= 0.5;
                break;
            }
            case PlayerState::Flying:
            {
                if (is_key_pressed(KeyCode::Space))
                {
                    m_position.y += FLYING_SPEED * delta;
                }
                if (is_mod_pressed(KeyMod::Ctrl))
                {
                    m_position.y -= FLYING_SPEED * delta;
                }
                break;
            }
            default:
            {
                DRK_ASSERT_DEBUG(false, "Unknown PlayerState");
                m_state = PlayerState::Normal;
            }
        }
        if (is_key_pressed(KeyCode::W)) m_position += speed * delta * horizontalDirection;
        if (is_key_pressed(KeyCode::S)) m_position -= speed * delta * horizontalDirection;
        if (is_key_pressed(KeyCode::A)) m_position -= speed * delta * glm::normalize(glm::cross(horizontalDirection, {0.0f, 1.0f, 0.0f}));
        if (is_key_pressed(KeyCode::D)) m_position += speed * delta * glm::normalize(glm::cross(horizontalDirection, {0.0f, 1.0f, 0.0f}));

        if (flying())
        m_camera.set_position(m_position + m_cameraOffset);
    }

    bool Player::on_event(const InputEvent& event)
    {
        switch (event.get_type())
        {
            case EventType::MouseMoved:
            {
                // m_direction
                m_camera.set_direction(m_direction);
                return false;
            }
            case EventType::MouseButtonPressed:
            {
                const auto& e = event_cast<MouseButtonEvent>(event);
                switch (e.button)
                {
                    case MouseCode::Left:
                    {
                        // Hit block / attack
                        return true;
                    }
                    case MouseCode::Right:
                    {
                        // Place block / use
                        return true;
                    }
                    default:
                        return false;
                }
            }
            case EventType::KeyPressed:
            {
                const auto& e = event_cast<KeyEvent>(event);
                switch (e.key)
                {
                    case KeyCode::Space:
                    {
                        jump();
                        return true;
                    }
                    case KeyCode::F:
                    {
                        if (CommandLineOptions::dev_mode_activated())
                        {
                            toggle_flying();
                            return true;
                        }
                        else
                            return false;
                    }
                    default:
                        return false;
                }
            }
            default:
                return false;
        }
    }

    void Player::render(void)
    {

    }

    bool Player::flying(void) const
    {
        return m_state == PlayerState::Flying;
    }

    const Camera& Player::get_camera(void) const
    {
        return m_camera;
    }

    void Player::jump(void)
    {
        if (m_state == PlayerState::Crouching)
            m_state = PlayerState::Normal;
    }

    void Player::toggle_flying(void)
    {
        if (flying())
            m_state = PlayerState::Normal;
        else
            m_state = PlayerState::Flying;
    }
}
