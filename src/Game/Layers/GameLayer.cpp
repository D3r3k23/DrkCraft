#include "GameLayer.hpp"

#include "Application/Application.hpp"
#include "Game/SavedWorldLoader.hpp"
#include "Game/Layers/PauseMenu.hpp"
#include "System/Input.hpp"
#include "Util/File.hpp"
#include "Core/Debug/Profiler.hpp"

#include <utility>

namespace DrkCraft
{
    GameLayer::GameLayer(void)
      : Layer("GameLayer", true),
        m_loadingScreen(Layer::create<LoadingScreen>()),
        m_hudLayer(Layer::create<Hud>()),
        m_consoleLayer(Layer::create<Console>()),
        m_debugLayer(Layer::create<DebugOverlay>()),
        m_startPaused(false)
    {
        Application::add_overlay(m_loadingScreen);
    }

    GameLayer::GameLayer(const fs::path& saveDir)
      : GameLayer()
    {
        DRK_PROFILE_FUNCTION();

        DRK_ASSERT_CORE(is_dir(saveDir), "Path \"{}\" is not a directory", saveDir.generic_string());
        DRK_LOG_GAME_INFO("Loading saved world from directory: {}", saveDir.generic_string());

        auto savedWorldLoader = make_ptr<SavedWorldLoader>(saveDir);

        DRK_PROFILE_THREAD_CREATE("saved_world_load");
        m_worldLoadThread = std::jthread([this, loader=std::move(savedWorldLoader)]
        {
            DRK_PROFILE_THREAD_START("saved_world_load");
            m_loadedWorld = loader->load();
        });
    }

    GameLayer::GameLayer(const WorldGeneratorSpec& worldGeneratorSpec)
      : GameLayer()
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_GAME_INFO("Generating world");

        auto worldGenerator = make_ptr<WorldGenerator>(worldGeneratorSpec);

        DRK_PROFILE_THREAD_CREATE("world_generation");
        m_worldLoadThread = std::jthread([this, generator=std::move(worldGenerator)]
        {
            DRK_PROFILE_THREAD_START("world_generation");
            m_loadedWorld = generator->generate();
        });

        // For now: Entire world will be in memory
        // For larger worlds: Generate World here and save to disk, then start
        //   game with saved world. Game uses WorldLoader to load/unload
    }

    GameLayer::~GameLayer(void)
    {

    }

    void GameLayer::set_game_start_callback_fn(const GameStartCallbackFn& fn)
    {
        m_onGameStart = fn;
    }

    void GameLayer::on_attach(void)
    {
        Application::add_overlay(m_hudLayer);
        Application::add_overlay(m_consoleLayer);
        Application::add_overlay(m_debugLayer);
    }

    void GameLayer::on_detach(void)
    {
        m_debugLayer->detach_layer();
        m_consoleLayer->detach_layer();
        m_hudLayer->detach_layer();
    }

    void GameLayer::on_update(Timestep timestep)
    {
        if (m_game)
        {
            m_game->update(timestep);
        }
        else
        {
            if (m_loadedWorld)
            {
                m_worldLoadThread.join();
                start_game();
            }
        }
    }

    void GameLayer::on_render(void)
    {
        if (m_game)
            m_game->render();
    }

    void GameLayer::on_event(Event& event)
    {
        DRK_PROFILE_FUNCTION();

        EventDispatcher ed(event);
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));
        ed.dispatch<WindowFocusLostEvent>(DRK_BIND_FN(on_window_focus_lost));
        ed.dispatch<MonitorEvent>(DRK_BIND_FN(on_monitor_event));

        if (m_game && event == EventCategory::Input)
            m_game->on_event(event_cast<InputEvent>(event));
    }

    bool GameLayer::on_key_pressed(const KeyPressedEvent& event)
    {
        switch (event.key)
        {
            case KeyCode::Escape:
            {
                pause_game();
                return true;
            }
            case KeyCode::F8:
            {
                if (m_game)
                    toggle_debug_overlay();
                return true;
            }
            default:
                return false;
        }
    }

    bool GameLayer::on_window_focus_lost(const WindowFocusLostEvent& event)
    {
        pause_game();
        return false;
    }

    bool GameLayer::on_monitor_event(const MonitorEvent& event)
    {
        pause_game();
        return false;
    }

    void GameLayer::start_game(void)
    {
        DRK_LOG_CORE_TRACE("GameLayer: Starting Game");

        m_loadingScreen->detach_layer();

        m_game = make_ref<Game>(std::move(m_loadedWorld), Application::get_assets());
        m_debugLayer->attach_game(m_game);

        if (m_startPaused)
            pause_game();
    }

    void GameLayer::exit_game(void)
    {
        detach_layer();
    }

    void GameLayer::pause_game(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_GAME_INFO("Paused");

        if (m_game)
        {
            if (!m_game->is_paused())
            {
                m_game->pause();
                open_pause_menu();
                deactivate_layer();
            }
        }
        else
            m_startPaused = true;
    }

    void GameLayer::unpause_game(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_GAME_INFO("Unpaused");

        if (m_game)
        {
            if (m_game->is_paused())
            {
                m_game->unpause();
                activate_layer();
            }
        }
        else
            m_startPaused = false;
    }

    void GameLayer::open_pause_menu(void)
    {
        auto pauseMenu = Layer::create<PauseMenu>();
        pauseMenu->set_unpause_callback_fn(DRK_BIND_FN(unpause_game));
        pauseMenu->set_exit_game_callback_fn(DRK_BIND_FN(exit_game));
        pauseMenu->set_save_game_callback_fn(DRK_BIND_FN(m_game->save));
        Application::add_layer(pauseMenu);
    }

    void GameLayer::show_debug_overlay(void)
    {
        if (!m_debugLayer->is_layer_active())
            m_debugLayer->activate_layer();
    }

    void GameLayer::hide_debug_overlay(void)
    {
        if (m_debugLayer->is_layer_active())
            m_debugLayer->deactivate_layer();
    }

    void GameLayer::toggle_debug_overlay(void)
    {
        if (!m_debugLayer->is_layer_active())
            show_debug_overlay();
        else
            hide_debug_overlay();
    }
}

