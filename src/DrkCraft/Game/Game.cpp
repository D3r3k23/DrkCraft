#include "Game.hpp"

#include "Application/Application.hpp"
#include "Graphics/Renderer/Renderer.hpp"
#include "Graphics/Renderer/BlockRenderer.hpp"
#include "Audio/Audio.hpp"
#include "System/Input.hpp"
#include "Core/Debug/Profiler.hpp"

#include <utility>

// Temp
#include <array>
#include <glad/glad.h>

namespace DrkCraft::Game
{
    static const AssetList s_REQUIRED_ASSETS
    {
        // { AssetType::Texture, "blockatlas.png" },
        // { AssetType::Texture, "skybox.png"     },

        { AssetType::Song, "Alix Perez - Burning Babylon.mp3" }
    };

    const AssetList& Game::get_asset_list(void)
    {
        return s_REQUIRED_ASSETS;
    }

    Game::Game(Library& assets)
      : m_assets(assets),
        m_world(),
        m_systemData(m_world, m_scene, m_gameEventQueue),
        m_worldRenderer(m_systemData),
        m_entityRenderer(m_systemData),
        m_lightingSystem(m_systemData),
        m_physicsSystem(m_systemData),
        m_playerController(create_player(m_scene)),
        m_hud(Layer::create<Hud>(m_playerController, true)),
        m_running(true),
        m_paused(false)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_GAME_INFO("Starting game");

        // const auto& blockAtlas = assets.get_texture("blockatlas.png");
        // BlockRenderer::set_texture_atlas(blockAtlas);

        Application::add_layer(m_hud);

        song = m_assets.get_song("Alix Perez - Burning Babylon.mp3");
    }

    Game::~Game(void)
    {
        m_hud->detach_layer();
    }

    void Game::render(void)
    {
        DRK_PROFILE_FUNCTION();

        Renderer::begin_scene(m_playerController.get_camera());

        m_sky.render();

        m_worldRenderer.render();
        m_entityRenderer.render();
        m_lightingSystem.render();
        m_physicsSystem.render();

        Renderer::end_scene();
    }

    void Game::update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        m_gameTime.update(timestep);

        m_sky.update(m_gameTime);
        m_playerController.update(timestep);

        m_worldRenderer.update(timestep);
        m_entityRenderer.update(timestep);
        m_lightingSystem.update(timestep);
        m_physicsSystem.update(timestep);

        m_gameEventQueue.poll();
    }

    void Game::on_event(InputEvent& event)
    {
        DRK_PROFILE_FUNCTION();

        EventDispatcher ed(event);
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));

        m_playerController.on_event(event);
    }

    bool Game::on_key_pressed(const KeyPressedEvent& event)
    {
        switch (event.key)
        {
            case KeyCode::F8:
            {
                if constexpr (DRK_DEBUG_ENABLED)
                {
                    // m_worldRenderer.show_wireframe(); // or inside DebugOverlay
                    return true;
                }
                else
                    return false;
            }
            case KeyCode::Space:
            {
                if (song->is_playing())
                {
                    DRK_LOG_GAME_INFO("Pausing song");
                    Audio::pause(song);
                }
                else
                {
                    DRK_LOG_GAME_INFO("Playing song");
                    Audio::play(song);
                }
                return true;
            }
            default:
                return false;
        }
    }

    void Game::pause(void)
    {
        DRK_LOG_GAME_INFO("Paused");

        m_paused = true;
        Audio::pause();
    }

    void Game::unpause(void)
    {
        DRK_LOG_GAME_INFO("Unpaused");

        m_paused = false;
        Audio::play();
    }

    bool Game::is_paused(void) const
    {
        return m_paused;
    }

    const PlayerController& Game::get_player(void) const
    {
        return m_playerController;
    }

    const GameTime& Game::get_time(void) const
    {
        return m_gameTime;
    }

    void Game::save(void)
    {
        DRK_LOG_GAME_INFO("Saving Game");
    }
}
