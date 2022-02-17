#ifndef DRK_GAME_LAYERS_GAME_LAYER_HPP
#define DRK_GAME_LAYERS_GAME_LAYER_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Application/Timestep.hpp"

#include "Game/Game.hpp"
#include "Game/World/WorldGenerator.hpp"
#include "Application/Layers/LoadingScreen.hpp"
#include "Game/Layers/Hud.hpp"
#include "Game/Layers/Console.hpp"
#include "Game/Layers/DebugOverlay.hpp"

#include "lib/fs.hpp"

#include <thread>
#include <atomic>
#include <functional>

namespace DrkCraft
{
    class GameLayer : public Layer
    {
        using GameStartCallbackFn = std::function<void(void)>;

    private:
        GameLayer(void);

    public:
        GameLayer(const fs::path& saveDir);
        GameLayer(const WorldGeneratorSpec& worldGeneratorSpec);

        virtual ~GameLayer(void);

        void set_game_start_callback_fn(const GameStartCallbackFn& fn);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_render(void) override;
        virtual void on_update(Timestep timestep) override;
        virtual void on_event(Event& event) override;

    private:
        virtual bool on_key_pressed(const KeyPressedEvent& event) override;
        virtual bool on_window_focus_lost(const WindowFocusLostEvent& event) override;
        virtual bool on_monitor_event(const MonitorEvent& event) override;

        void start_game(void);
        void exit_game(void);

        void pause_game(void);
        void unpause_game(void);
        void open_pause_menu(void);

        void show_debug_overlay(void);
        void hide_debug_overlay(void);
        void toggle_debug_overlay(void);

    private:
        Ref<Game> m_game;

        Ref<LoadingScreen> m_loadingScreen;

        Ref<Hud>    m_hudLayer;
        Ref<Console>   m_consoleLayer;
        Ref<DebugOverlay> m_debugLayer;

        std::jthread m_worldLoadThread;
        Ptr<World> m_loadedWorld;
        std::atomic<bool> m_worldLoaded;

        GameStartCallbackFn m_onGameStart;

        bool m_startPaused;
    };
}

#endif // DRK_GAME_LAYERS_GAME_LAYER_HPP
