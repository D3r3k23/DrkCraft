#include "Player.hpp"

#include "Game/Entity/Components.hpp"
#include "System/Input.hpp"
#include "Core/Settings.hpp"
#include "Core/Debug/Profiler.hpp"

#include <entt/entity/registry.hpp>

#include <glm/geometric.hpp>

namespace DrkCraft
{
    static constexpr float PLAYER_BASE_SPEED = 1.0f;
    static constexpr float SPRINT_SPEED_MULT = 1.5f;
    static constexpr float CROUCH_SPEED_MULT = 0.5f;
    static constexpr float FLYING_SPEED_MULT = 2.0f;
    static constexpr float FLYING_SPEED_VERT = 1.5f;

    Entity create_player(EntityScene& entityScene)
    {
        Entity entity = entityScene.create();
        entity.add_component<PlayerComponent>();
        entity.add_component<TransformComponent>();
        entity.add_component<HealthComponent>();
        return entity;
    }

    PlayerController::PlayerController(Entity playerEntity)
      : m_entity(playerEntity),
        m_state(PlayerState::Normal),
        m_position(0.0f, 0.0f, -2.0f),
        m_direction(0.0f, 0.0f, 1.0f),
        m_distanceAboveGround(0.0f),
        m_camera(45.0f, 16.9f, 0.01f, 1000.0f, m_position, m_direction),
        m_cameraOffset(0.0f, 2.0f, 0.0f)
    {
        DRK_ASSERT_DEBUG(playerEntity.has_component<PlayerComponent>(), "Entity is not a valid Player");
    }

    void PlayerController::update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

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
                break;
            }
            case Crouching:
            {
                if (!is_pressed(keybinds.player_movement.crouch))
                {
                    m_state = Normal;
                }
                break;
            }
            case Flying:
            {
                break;
            }
            default:
            {
                DRK_ASSERT_DEBUG(false, "Unknown PlayerState");
                m_state = Normal;
            }
        }

        float speed = PLAYER_BASE_SPEED;
        switch (m_state)
        {
            case Sprinting : speed *= SPRINT_SPEED_MULT; break;
            case Crouching : speed *= CROUCH_SPEED_MULT; break;
            case Flying    : speed *= FLYING_SPEED_MULT; break;
        }

        const vec3 horizontalForwardDirection = glm::normalize(vec3{m_direction.x, m_direction.y, 0.0});
        const vec3 horizontalRightDirection   = glm::normalize(glm::cross(horizontalForwardDirection, {0.0f, 1.0f, 0.0f}));

        vec3 d_position{0.0f};
        if (is_pressed(keybinds.player_movement.forward)) d_position += speed * timestep * horizontalForwardDirection;
        if (is_pressed(keybinds.player_movement.back))    d_position -= speed * timestep * horizontalForwardDirection;
        if (is_pressed(keybinds.player_movement.left))    d_position -= speed * timestep * horizontalRightDirection;
        if (is_pressed(keybinds.player_movement.right))   d_position += speed * timestep * horizontalRightDirection;

        if (is_flying())
        {
            if (is_pressed(keybinds.player_movement.jump))   d_position.y += FLYING_SPEED_VERT * timestep;
            if (is_pressed(keybinds.player_movement.crouch)) d_position.y -= FLYING_SPEED_VERT * timestep;
        }

        m_position += d_position;
        m_entity.get_component<TransformComponent>().transform.translate(d_position);

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
        // Can never look straight up

        // m_camera.set_direction(m_direction);
        // m_camera.tilt()
        // m_camera.rotate()
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
        const auto eventCode = event.get_input_code();
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
            if (!is_flying())
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

    vec3 PlayerController::get_position(void) const
    {
        return m_position;
    }


    bool PlayerController::is_flying(void) const
    {
        return m_state == PlayerState::Flying;
    }

    const Camera& PlayerController::get_camera(void) const
    {
        return m_camera;
    }

    Camera& PlayerController::get_camera(void)
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
        {
            m_state = PlayerState::Flying;
            // Disable gravity for physics system
        }
    }
}
