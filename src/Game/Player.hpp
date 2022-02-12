#ifndef DRK_GAME_PLAYER_HPP
#define DRK_GAME_PLAYER_HPP

#include "Core/Base.hpp"
#include "Graphics/Camera.hpp"
#include "Application/Events.hpp"
#include "Application/Timestep.hpp"

#include "lib/glm/vec3.hpp"

namespace DrkCraft
{
    enum class PlayerState
    {
        Normal,
        Sprinting,
        Crouching,
        Flying
    };

    class Player
    {
    public:
        Player(void);

        void update(Timestep timestep);
        bool on_event(const InputEvent& event);
        void render(void);

        void jump(void);

        bool flying(void) const;

        const Camera& get_camera(void) const;

    private:
        void jump(void);
        void toggle_flying(void);

    private:
        vec3 m_position;
        vec3 m_direction; // Horizontal
        PlayerState m_state;

        float m_distanceAboveGround;

        Camera m_camera;
        vec3 m_cameraOffset;
    };
}

#endif // DRK_GAME_PLAYER_HPP
