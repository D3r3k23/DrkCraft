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

    void Player::render(void)
    {

    }

    void Player::update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        const float delta{timestep};
        const vec3 horizontalDirection = {m_direction.x, m_direction.y, 0.0};

        float speed = 1.0f;

        const KeyBinds keybinds = RuntimeSettings::keybinds();

        using enum PlayerState;
        switch (m_state)
        {
            case Normal:
            {
                if (is_key_pressed(keybinds.player_movement.sprint))
                {
                    m_state = Sprinting;
                }
                else if (is_key_pressed(keybinds.player_movement.crouch))
                {
                    m_state = Crouching;
                }
                break;
            }
            case Sprinting:
            {
                if (!is_key_pressed(keybinds.player_movement.sprint))
                {
                    m_state = Normal;
                }
                else if (is_key_pressed(keybinds.player_movement.crouch))
                {
                    m_state = Crouching;
                }
                speed *= 1.5;
                break;
            }
            case Crouching:
            {
                if (!is_key_pressed(keybinds.player_movement.crouch))
                {
                    m_state = Normal;
                }
                speed *= 0.5;
                break;
            }
            case Flying:
            {
                if (is_key_pressed(keybinds.player_movement.jump))
                {
                    m_position.y += FLYING_SPEED * delta;
                }
                if (is_key_pressed(keybinds.player_movement.crouch))
                {
                    m_position.y -= FLYING_SPEED * delta;
                }
                break;
            }
            default:
            {
                DRK_ASSERT_DEBUG(false, "Unknown PlayerState");
                m_state = Normal;
            }
        }
        if (is_key_pressed(keybinds.player_movement.forward)) m_position += speed * delta * horizontalDirection;
        if (is_key_pressed(keybinds.player_movement.back))    m_position -= speed * delta * horizontalDirection;
        if (is_key_pressed(keybinds.player_movement.left))    m_position -= speed * delta * glm::normalize(glm::cross(horizontalDirection, {0.0f, 1.0f, 0.0f}));
        if (is_key_pressed(keybinds.player_movement.right))   m_position += speed * delta * glm::normalize(glm::cross(horizontalDirection, {0.0f, 1.0f, 0.0f}));

        if (flying());

        m_camera.set_position(m_position + m_cameraOffset);
    }

    void Player::on_event(InputEvent& event)
    {
        EventDispatcher ed(event);
        ed.dispatch<MouseMovedEvent>(DRK_BIND_FN(on_mouse_moved));
        ed.dispatch<MouseButtonPressedEvent>(DRK_BIND_FN(on_mouse_button_pressed));
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));
    }

    bool Player::on_mouse_moved(const MouseMovedEvent& event)
    {
        // m_direction

        m_camera.set_direction(m_direction);
        return false;
    }

    bool Player::on_mouse_button_pressed(const MouseButtonPressedEvent& event)
    {
        const KeyBinds keybinds = RuntimeSettings::keybinds();

        if (event.button == keybinds.player_actions.use)
        {
            return true;
        }
        else if (event.button == keybinds.player_actions.place)
        {
            return true;
        }
        else
            return false;
    }

    bool Player::on_key_pressed(const KeyPressedEvent& event)
    {
        const KeyBinds keybinds = RuntimeSettings::keybinds();

        if (event.key == keybinds.player_movement.jump)
        {
            jump();
            return true;
        }
        else if (event.key == keybinds.player_actions.interact)
        {
            if (1)
                return true;
            else
                return false;
        }
        else if (event.key == keybinds.player_actions.inventory)
        {
            return true;
        }
        else if (event.key == keybinds.player_actions.fly)
        {
            if (CommandLineOptions::dev_mode_activated())
            {
                toggle_flying();
                return true;
            }
            else
                return false;
        }
        else
            return false;
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
