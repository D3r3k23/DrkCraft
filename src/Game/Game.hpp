#ifndef DRK_GAME_HPP
#define DRK_GAME_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Timestep.hpp"
#include "Application/Events.hpp"
#include "HudLayer.hpp"
#include "DebugLayer.hpp"
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
        ~Game(void);

        void on_attach(void) override;
        void on_detach(void) override;

        void on_update(Timestep timestep) override;
        void on_render(Timestep timestep) override;
        void on_event(Event& event) override;

    private:
        bool on_key_pressed(KeyPressedEvent& event);

        void pause(void);
        void unpause(void);
        bool is_paused(void) const;

        void save(void);

    private:
        Scene scene;
        World world;

        Ref<HudLayer> m_hudLayer;
        Ref<DebugLayer> m_debugLayer;

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
