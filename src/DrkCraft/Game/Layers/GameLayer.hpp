#ifndef DRK_GAME_LAYERS_GAME_LAYER_HPP
#define DRK_GAME_LAYERS_GAME_LAYER_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Util/Timestep.hpp"

#include "Game/Game.hpp"
#include "System/Library.hpp"
#include "Game/Save/SaveManager.hpp"
#include "Game/Save/SaveViewer.hpp"
#include "Game/World/WorldGenerator.hpp"

#include "Game/Layers/DebugOverlay.hpp"
#include "Application/Layers/LoadingScreen.hpp"
#include "System/Thread.hpp"

#include "Lib/fs.hpp"
#include "Lib/string_view.hpp"

#include <atomic>

namespace DrkCraft::Game
{
    enum class GameLayerState
    {
        Init = 0,
        Ready,
        LoadingGame,
        LoadingComplete,
        RunningGame,
        SavingGame,
        SavingComplete,
        Closed
    };

    /* Startup Procedue:
    - Layer begins unattached
    - Since ctor starts loading both assets and saves on other threads:
      - MainMenu will wait until both are complete before loading game
        * By checking is_intializing()
      - MainMenu can still acquire a SaveViewer from SaveManager while it's loading
        * SaveManager has map of all save data (i.e. not chunk & entity data)
        * SaveViewer has vector of SaveInfo* to SaveManager data
    - When load_game() is called, the game begins loading on another thread
      - Now the Layer can be attached, and the loading screen will be displayed until
          the game is ready
    */

    class GameLayer : public Layer
    {
    private:
        using State = GameLayerState;

    public:
        GameLayer(fs::path saveDir);
        virtual ~GameLayer(void);

        void load_assets(void);
        Result<> load_game(Ptr<SaveLoader> saveLoader);
        Result<> load_game(string_view newSaveName, const WorldGeneratorSpec& worldSpec);

        bool is_initializing(void) const;
        bool is_game_running(void) const;

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_render(void) override;
        virtual void on_update(Timestep timestep) override;
        virtual void on_event(Event& event) override;

    private:
        virtual bool on_key_pressed(const KeyPressedEvent& event) override;
        virtual bool on_window_closed(const WindowClosedEvent& event) override;
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
        std::atomic<State> m_state;

        Library& m_library;
        Optional<Game> m_game;

        fs::path m_saveDir;

        Ref<LoadingScreen> m_loadingScreen;
        Ref<DebugOverlay> m_debugOverlay;

        Ptr<WorldGenerator> m_worldGenerator;

        Thread<> m_gameLoadThread;
        Ptr<World> m_loadedWorld;
        std::atomic<bool> m_worldLoaded;

        bool m_startPaused;
    };
}

#endif // DRK_GAME_LAYERS_GAME_LAYER_HPP
