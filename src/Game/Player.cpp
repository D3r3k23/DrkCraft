#include "Player.hpp"

#include "Game/Entity/Components.hpp"
#include "System/Input.hpp"
#include "Core/Settings.hpp"
#include "Core/Debug/Profiler.hpp"

#include <entt/entity/registry.hpp>

#include <glm/geometric.hpp>

namespace DrkCraft
{
    static constexpr float FLYING_SPEED = 1.0f;

    Entity create_player(EntityScene& entityScene)
    {
        Entity entity = entityScene.create();
        entity.add_component<PlayerComponent>();
        return entity;
    }

    PlayerController::PlayerController(Entity playerEntity)
      : m_entity(playerEntity),
        m_position(0.0f, 0.0f, -2.0f),
        m_direction(0.0f, 0.0f, 1.0f),
        m_state(PlayerState::Normal),
        m_distanceAboveGround(0.0f),
        m_camera(45.0f, 16.9f, 0.01f, 1000.0f, m_position, m_direction),
        m_cameraOffset(0.0f, 2.0f, 0.0f)
    {
        DRK_ASSERT_DEBUG(playerEntity.has_component<PlayerComponent>(), "Entity is not a valid Player");
    }

    void PlayerController::render(void)
    {

    }

    void PlayerController::update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        const float delta{timestep};
        const vec3 horizontalDirection = {m_direction.x, m_direction.y, 0.0};

        float speed = 1.0f;

        const KeyBinds keybinds = RuntimeSettings::get_keybinds();

        using enum PlayerState;
        switch (m_state)
        {
            case Normal:
            {
                if (is_pressed(keybinds.player_movement.sprint))
                {
                    m_state = Sprinting;
                }
                else if (is_pressed(keybinds.player_movement.crouch))
                {
                    m_state = Crouching;
                }
                break;
            }
            case Sprinting:
            {
                if (!is_pressed(keybinds.player_movement.sprint))
                {
                    m_state = Normal;
                }
                else if (is_pressed(keybinds.player_movement.crouch))
                {
                    m_state = Crouching;
                }
                speed *= 1.5;
                break;
            }
            case Crouching:
            {
                if (!is_pressed(keybinds.player_movement.crouch))
                {
                    m_state = Normal;
                }
                speed *= 0.5;
                break;
            }
            case Flying:
            {
                if (is_pressed(keybinds.player_movement.jump))
                {
                    m_position.y += FLYING_SPEED * delta;
                }
                if (is_pressed(keybinds.player_movement.crouch))
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
        if (is_pressed(keybinds.player_movement.forward)) m_position += speed * delta * horizontalDirection;
        if (is_pressed(keybinds.player_movement.back))    m_position -= speed * delta * horizontalDirection;
        if (is_pressed(keybinds.player_movement.left))    m_position -= speed * delta * glm::normalize(glm::cross(horizontalDirection, {0.0f, 1.0f, 0.0f}));
        if (is_pressed(keybinds.player_movement.right))   m_position += speed * delta * glm::normalize(glm::cross(horizontalDirection, {0.0f, 1.0f, 0.0f}));

        if (is_flying());

        m_camera.set_position(m_position + m_cameraOffset);
    }

    void PlayerController::on_event(InputEvent& event)
    {
        EventDispatcher ed(event);
        ed.dispatch<MouseMovedEvent>(DRK_BIND_FN(on_mouse_moved));
        ed.dispatch<MouseButtonPressedEvent>(DRK_BIND_FN(on_mouse_button_pressed));
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));
    }

    bool PlayerController::on_mouse_moved(const MouseMovedEvent& event)
    {
        // m_direction

        m_camera.set_direction(m_direction);
        return false;
    }

    bool PlayerController::on_mouse_button_pressed(const MouseButtonPressedEvent& event)
    {
        if (on_mouse_or_key_pressed(event))
            return true;

        return false;
    }

    bool PlayerController::on_key_pressed(const KeyPressedEvent& event)
    {
        if (on_mouse_or_key_pressed(event))
            return true;

        return false;
    }

    bool PlayerController::on_mouse_or_key_pressed(const InputEvent& event)
    {
        const auto eventCode = [&]() -> InputCode
        {
            if (event == EventType::KeyPressed)
                return event_cast<KeyPressedEvent>(event).key;
            else if (event == EventType::MouseButtonPressed)
                return event_cast<MouseButtonPressedEvent>(event).button;
            else
                return KeyCode::None;
        }();
        const auto& keybinds = RuntimeSettings::get_keybinds();

        if (eventCode == keybinds.player_actions.use)
        {
            return true;
        }
        else if (eventCode == keybinds.player_actions.place)
        {
            return true;
        }
        else if (eventCode == keybinds.player_movement.jump)
        {
            jump();
            return true;
        }
        else if (eventCode == keybinds.player_actions.interact)
        {
            if (1)
                return true;
            else
                return false;
        }
        else if (eventCode == keybinds.player_actions.inventory)
        {
            return true;
        }
        else if (eventCode == keybinds.player_actions.fly)
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

    bool PlayerController::is_flying(void) const
    {
        return m_state == PlayerState::Flying;
    }

    const Camera& PlayerController::get_camera(void) const
    {
        return m_camera;
    }

    void PlayerController::jump(void)
    {
        if (m_state == PlayerState::Crouching)
            m_state = PlayerState::Normal;
    }

    void PlayerController::toggle_flying(void)
    {
        if (is_flying())
            m_state = PlayerState::Normal;
        else
            m_state = PlayerState::Flying;
    }
}
