#ifndef DRK_GAME_HPP
#define DRK_GAME_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Timestep.hpp"
#include "Application/Events.hpp"
#include "Engine/Scene.hpp"
#include "World.hpp"

// Temp
#include "Core/Util.hpp"
#include "Graphics/Renderer.hpp"
#include <array>

namespace DrkCraft
{
    class Game : public Layer
    {
    public:
        Game(void);

        void on_update(Timestep timestep) override;
        void on_render(Timestep timestep) override;
        void on_event(Event& event) override;

        bool on_key_pressed(KeyPressedEvent& event);

        void pause(void);

    private:
        Scene scene;
        World world;

        bool m_running;
        bool m_paused;

        // Temp
        GLuint vertexArrayObject;
        ShaderProgram flatColorShaderProgram;
        glm::vec3 color;
        RandomFloatDist randomDist;
    };
}

#endif // DRK_GAME_HPP
