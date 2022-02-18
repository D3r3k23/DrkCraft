#ifndef DRK_GAME_PLAYER_HPP
#define DRK_GAME_PLAYER_HPP

#include "Core/Base.hpp"
#include "Graphics/Camera.hpp"
#include "Application/Events.hpp"
#include "Application/Timestep.hpp"
#include "Game/Entity/Entity.hpp"
#include "Game/Entity/EntityScene.hpp"

#include "lib/glm/vec3.hpp"

namespace DrkCraft
{
    Entity create_player(EntityScene& entityScene);

    enum class PlayerState
    {
        Normal,
        Sprinting,
        Crouching,
        Flying
    };

    class PlayerController
    {
    public:
        PlayerController(Entity playerEntity);

        void render(void);
        void update(Timestep timestep);
        void on_event(InputEvent& event);

        bool on_mouse_moved(const MouseMovedEvent& event);
        bool on_mouse_button_pressed(const MouseButtonPressedEvent& event);
        bool on_key_pressed(const KeyPressedEvent& event);
        bool on_mouse_or_key_pressed(const InputEvent& event);

        void jump(void);

        bool is_flying(void) const;

        const Camera& get_camera(void) const;

    private:
        void jump(void);
        void toggle_flying(void);

    private:
        Entity m_entity;

        vec3 m_position;
        vec3 m_direction; // Horizontal
        PlayerState m_state;

        float m_distanceAboveGround;

        Camera m_camera;
        vec3 m_cameraOffset;
    };
}

#endif // DRK_GAME_PLAYER_HPP
