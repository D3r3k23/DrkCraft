#include "GameLayer.hpp"

#include "Application/Application.hpp"
#include "Game/Save.hpp"
#include "Game/Layers/PauseMenu.hpp"
#include "System/Input.hpp"
#include "Disk/File.hpp"
#include "Core/Debug/Profiler.hpp"

namespace DrkCraft::Game
{
    GameLayer::GameLayer(fs::path saveDir)
      : Layer("GameLayer"),
        m_state(State::Init),
        m_library(Application::get_library()),
        m_saveDir(std::move(saveDir)),
        m_startPaused(false)
    {
        DRK_PROFILE_FUNCTION();

        if (loadAssets)
            load_assets();
    }

    // GameLayer::GameLayer(Ptr<SaveLoader> saveLoader, bool loadAssets)
    //   : GameLayer(loadingScreen)
    // {
    //     DRK_PROFILE_FUNCTION();

    //     DRK_ASSERT_CORE(is_dir(saveDir), "Path \"{}\" is not a directory", saveDir.generic_string());
    //     DRK_LOG_GAME_INFO("Loading saved game from directory: {}", saveDir.generic_string());

    //     loadingScreen->set_message("Loading Save");
    //     auto saveLoader = make_ptr<SaveLoader>(saveDir);

    //     m_gameLoadThread = Thread<>("saved_game_load_thread", [this, loader=std::move(saveLoader)]
    //     {
    //         m_loadedWorld = loader->load();
    //         m_worldLoaded = true;
    //     });
    // }

    // GameLayer::GameLayer(const WorldGeneratorSpec& worldGeneratorSpec, bool loadAssets)
    //   : GameLayer(loadingScreen)
    // {
    //     DRK_PROFILE_FUNCTION();

    //     DRK_LOG_GAME_INFO("Generating world");

    //     loadingScreen->set_message("Generating world");
    //     auto worldGenerator = make_ptr<WorldGenerator>(worldGeneratorSpec);

    //     m_gameLoadThread = Thread<>("world_generation_thread", [this, generator=std::move(worldGenerator)]
    //     {
    //         m_loadedWorld = generator->generate();
    //         m_worldLoaded = true;
    //     });

    //     // For now: Entire world will be in memory
    //     // For larger worlds: Generate World here and save to disk, then start
    //     //   game with saved world. Game uses WorldLoader to load/unload
    // }

    GameLayer::~GameLayer(void)
    {

    }

    void GameLayer::load_assets(void)
    {
        DRK_PROFILE_FUNCTION();
        m_state = State::LoadingAssets;

        // Load assets w/ callback:
        m_state = State::Ready;
    }

    // In these threads: wait for assets to finish loading

    Result<> GameLayer::load_game(Ptr<SaveLoader> saveLoader)
    {
        DRK_PROFILE_FUNCTION();
        if (!ready_to_load_game())
            return ResultStatus::Failure;
        else
            DRK_LOG_CORE_INFO("GameLayer: Loading save");

        m_gameLoadThread = Thread<>("load_save_thread", [&]
        {
            m_loadingScreen->set_message("Loading save");
            const auto save = saveLoader->load();

            // get worldSpec from SaveData::World

            // m_worldGenerator = WorldGenerator::load(worldSpec);
            // m_worldLoader = make_ptr<WorldLoader>(*m_worldGenerator, WorldLoader::Task::LoadSave);

            // m_loadingScreen->set_message("Loading world");
            // Now we wait for m_worldLoader.status<Task::LoadSave>() to be Task::LoadSave::Complete <- this is determined by the strategy

            m_state = State::LoadingComplete;
        });
        return ResultStatus::Success;
    }

    Result<> GameLayer::load_game(string_view newSaveName, const WorldGeneratorSpec& worldSpec)
    {
        DRK_PROFILE_FUNCTION();
        if (!ready_to_load_game())
            return ResultStatus::Failure;
        else
            DRK_LOG_CORE_INFO("GameLayer: Creating new save");

        m_gameLoadThread = Thread<>("new_save_thread", [&]
        {
            m_loadingScreen->set_message("Creating save");
            auto saveLoader = create_new_save(m_saveDir / newSaveName);

            // m_worldGenerator = WorldGenerator::create(worldSpec);
            // m_worldLoader = make_ptr<WorldLoader>(*m_worldGenerator, WorldLoader::Task::CreateWorld);

            // m_loadingScreen->set_message("Generating world");
            // Now wait for m_worldLoader.status<CreateWorld>() == CreateWorld::Complete

            m_loadingScreen->set_message("Loading new save");
            const auto save = saveLoader->load();

            m_state = State::LoadingComplete;
        });
        return ResultStatus::Success;
    }

    bool GameLayer::ready_to_load_game(void) const
    {
        return m_state == State::Ready;
    }

    bool GameLayer::is_game_running(void) const
    {
        return (bool)m_game;
    }

    void GameLayer::on_attach(void)
    {

    }

    void GameLayer::on_detach(void)
    {

    }

    void GameLayer::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        if (is_game_running())
        {
            m_game->render();
        }
    }

    void GameLayer::on_update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        if (is_game_running())
        {
            m_game->update(timestep);
        }
        else
            switch (m_state)
            {
                case State::Init:
                    load_assets();
                    break;
                case State::LoadingAssets:
                    if (!m_loadingScreen)
                    {
                        m_loadingScreen = Layer::create<LoadingScreen>("Loading Assets");
                        Application::add_overlay(m_loadingScreen);
                    }
                    break;
                case State::Ready:
                    break;
                case State::LoadingGame:
                    break;
                case State::LoadingComplete:
                    m_gameLoadThread.join();
                    start_game();
                    break;
                case State::RunningGame:
                    break;
                case State::SavingGame:
                    break;
                case State::SavingComplete:
                    break;
                case State::Closed:
                    break;
                default:
                    DRK_ASSERT_DEBUG_FALSE("Invalid GameLayerState: {}", m_state);
                    break;
            }
    }

    void GameLayer::on_event(Event& event)
    {
        DRK_PROFILE_FUNCTION();

        EventDispatcher ed(event);

        ed.dispatch<WindowClosedEvent>(DRK_BIND_FN(on_window_closed));
        ed.dispatch<WindowFocusLostEvent>(DRK_BIND_FN(on_window_focus_lost));
        ed.dispatch<MonitorEvent>(DRK_BIND_FN(on_monitor_event));

        if (is_game_running())
        {
            ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));

            if (event == EventCategory::Input)
                m_game->on_event(event_cast<InputEvent>(event));
        }
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
            case KeyCode::F5:
            {
                // Take screenshot (make screenshot directories if not exist)
                return true;
            }
            case KeyCode::F6:
            {
                toggle_debug_overlay();
                return true;
            }
            default:
                return false;
        }
    }

    bool GameLayer::on_window_closed(const WindowClosedEvent& event)
    {
        if (is_game_running())
            exit_game();

        return false;
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
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("GameLayer: Starting Game");

        DRK_ASSERT_DEBUG_NO_MSG(!is_game_running());

        if (m_loadingScreen)
        {
            m_loadingScreen->detach_layer();
            m_loadingScreen.reset();
        }

        m_game.emplace(m_library);

        m_debugOverlay = Layer::create<DebugOverlay>(*m_game);
        Application::add_overlay(m_debugOverlay);

        if (m_startPaused)
            pause_game();
    }

    void GameLayer::exit_game(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("GameLayer: Exiting Game");

        DRK_ASSERT_DEBUG_NO_MSG(is_game_running());

        m_debugOverlay->detach_layer();
        detach_layer();
        m_state = State::Closed;
    }

    void GameLayer::pause_game(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_GAME_INFO("Game paused");

        if (is_game_running())
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
        DRK_LOG_GAME_INFO("Game unpaused");

        if (is_game_running())
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
        if (!m_debugOverlay->is_layer_active())
            m_debugOverlay->activate_layer();
    }

    void GameLayer::hide_debug_overlay(void)
    {
        if (m_debugOverlay->is_layer_active())
            m_debugOverlay->deactivate_layer();
    }

    void GameLayer::toggle_debug_overlay(void)
    {
        if (!m_debugOverlay->is_layer_active())
            show_debug_overlay();
        else
            hide_debug_overlay();
    }
}
