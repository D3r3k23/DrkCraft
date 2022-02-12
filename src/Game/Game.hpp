#ifndef DRK_GAME_GAME_HPP
#define DRK_GAME_GAME_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Application/Timestep.hpp"
#include "System/AssetManager.hpp"
#include "Game/Layers/Hud.hpp"
#include "Game/Layers/Console.hpp"
#include "Game/Layers/DebugOverlay.hpp"
#include "Game/World.hpp"

// Temp
#include "Util/Random.hpp"
#include "Graphics/Util.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Buffer.hpp"
#include "System/Audio/AudioSource.hpp"

#include "lib/fs.hpp"

#include <optional>

namespace DrkCraft
{
    class Game : public Layer
    {
    private:
        Game(void);

    public:
        Game(const fs::path& saveDir);
        Game(const WorldGenerator& worldGenerator);

    public:
        virtual ~Game(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_update(Timestep timestep) override;
        virtual void on_render(void) override;
        virtual void on_event(Event& event) override;

        virtual bool on_key_pressed(const KeyPressedEvent& event) override;
        virtual bool on_window_focus_lost(const WindowFocusLostEvent& event) override;
        virtual bool on_monitor_event(const MonitorEvent& event) override;

        void pause(void);
        void unpause(void);
        bool is_paused(void) const;

        void open_pause_menu(void);

        void save(void);

    private:
        AssetManager& m_assets;

        Ptr<World> m_world;
        Ptr<WorldGenerator> m_worldGenerator;

        Ref<Hud> m_hudLayer;
        Ref<Console> m_consoleLayer;
        Ref<DebugOverlay> m_debugLayer;

        Player m_player;

        bool m_running;
        bool m_paused;

        // Temp
        Ptr<VertexBuffer> vertexBuffer;
        ShaderProgram flatColorShaderProgram;
        vec3 color;
        RandomFloatDist randomDist;

        Ref<AudioSource> song;
    };
}

#endif // DRK_GAME_GAME_HPP
