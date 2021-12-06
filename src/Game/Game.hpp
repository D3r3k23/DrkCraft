#ifndef DRK_GAME_HPP
#define DRK_GAME_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Timestep.hpp"
#include "Application/Events.hpp"
#include "Hud.hpp"
#include "Console.hpp"
#include "DebugOverlay.hpp"
#include "Engine/World.hpp"

// Temp
#include "Core/Util.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include <array>

namespace DrkCraft
{
    class Game : public Layer
    {
    public:
        Game(void);
        virtual ~Game(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_update(Timestep timestep) override;
        virtual void on_render(void) override;
        virtual void on_event(Event& event) override;

    private:
        virtual bool on_key_pressed(const KeyPressedEvent& event) override;
        virtual bool on_window_focus_lost(const WindowFocusLostEvent& event) override;

        void pause(void);
        void unpause(void);
        bool is_paused(void) const;

        void open_pause_menu(void);

        void save(void);

    private:
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
