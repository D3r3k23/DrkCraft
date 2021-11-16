#ifndef DRK_GAME_HPP
#define DRK_GAME_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Timestep.hpp"
#include "Application/Events.hpp"
#include "Hud.hpp"
#include "Console.hpp"
#include "DebugOverlay.hpp"
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
        bool on_key_pressed(const KeyPressedEvent& event);
        bool on_window_focus_lost(const WindowFocusLostEvent& event);

        void pause(void);
        void unpause(void);
        bool is_paused(void) const;

        void open_pause_menu(void);

        void save(void);

    private:
        Scene m_scene;
        World m_world;

        Ref<Hud> m_hudLayer;
        Ref<Console> m_consoleLayer;
        Ref<DebugOverlay> m_debugLayer;

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
