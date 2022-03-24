#ifndef DRK_GAME_PLAYER_HPP
#define DRK_GAME_PLAYER_HPP

#include "Core/Base.hpp"
#include "Application/Events.hpp"
#include "Core/Timestep.hpp"
#include "Game/Entity/Entity.hpp"
#include "Game/Entity/EntityScene.hpp"
#include "Graphics/Camera.hpp"
#include "Game/Items/Inventory.hpp"

#include "lib/glm/vec3.hpp"

namespace DrkCraft
{
    Entity create_player(EntityScene& entityScene);

    enum class PlayerState
    {
        Normal = 0,
        Sprinting,
        Crouching,
        Flying
    };

    class PlayerController
    {
    public:
        PlayerController(Entity playerEntity);

        void update(Timestep timestep);
        void on_event(InputEvent& event);

        bool on_mouse_moved(const MouseMovedEvent& event);
        bool on_mouse_button_pressed(const MouseButtonPressedEvent& event);
        bool on_key_pressed(const KeyPressedEvent& event);
        bool on_mouse_or_key_pressed(const InputEvent& event);

        vec3 get_position(void) const;

        bool is_flying(void) const;

        const Camera& get_camera(void) const;
        Camera& get_camera(void);

    private:
        void jump(void);
        void toggle_flying(void);

    private:
        Entity m_entity;
        PlayerState m_state;

        Inventory m_inventory;

        vec3 m_position;
        vec3 m_direction;

        float m_distanceAboveGround;

        Camera m_camera;
        vec3 m_cameraOffset;
    };
}

#endif // DRK_GAME_PLAYER_HPP
