#include "Game.hpp"

#include "Application/Application.hpp"
#include "PauseMenu.hpp"
#include "System/Input.hpp"
#include "Graphics/Transform.hpp"
#include "Core/Profiler.hpp"

// Temp
#include "Audio/Audio.hpp"
#include <array>

namespace DrkCraft
{
    Game::Game(void)
      : Layer("GameLayer", true),
        m_assets(Application::get_assets()),
        flatColorShaderProgram("FlatColorShaderProgram"),
        color(0.5f, 0.5f, 0.5f),
        randomDist(0.0f, 1.0f)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_GAME_INFO("Starting game");

        bool showHud = false;

        m_hudLayer     = Layer::create<Hud>(showHud);
        m_consoleLayer = Layer::create<Console>();
        m_debugLayer   = Layer::create<DebugOverlay>();

        std::array<float, 9> vertexPositions
        {
             0.0f,  0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
        };

        vertexBuffer = make_ptr<VertexBuffer>(vertexPositions);

        vertexBuffer->bind();
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        vertexBuffer->unbind();

        flatColorShaderProgram.add_shader(Shader::create(
            shader_asset_path("flat_color_vertex_shader.glsl"), ShaderType::Vertex));
        flatColorShaderProgram.add_shader(Shader::create(
            shader_asset_path("flat_color_fragment_shader.glsl"), ShaderType::Fragment));
        flatColorShaderProgram.link();

        song = m_assets.get_song("Alix Perez - Burning Babylon.mp3");
    }

    Game::~Game(void)
    {

    }

    void Game::on_attach(void)
    {
        DRK_PROFILE_FUNCTION();

        Application::get_instance().add_layer(m_hudLayer);
        Application::get_instance().add_overlay(m_consoleLayer);
        Application::get_instance().add_overlay(m_debugLayer);
    }

    void Game::on_detach(void)
    {
        DRK_PROFILE_FUNCTION();

        m_debugLayer->detach_layer();
        m_consoleLayer->detach_layer();
        m_hudLayer->detach_layer();
    }

    void Game::on_update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        m_player.on_update(timestep);
    }

    void Game::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        GlObjectHandler<ShaderProgram> shader(flatColorShaderProgram);
        // flatColorShaderProgram.upload_uniform_mat("u_viewProjection", m_player.get_view_projection());
        // flatColorShaderProgram.upload_uniform_mat("u_transform", Transform::Identity());
        flatColorShaderProgram.upload_uniform_vec4("u_color", glm::vec4(color, 1.0f));

        Renderer::draw_triangle(*vertexBuffer);
        // Renderer::draw_block(0, 0, 0);
    }

    void Game::on_event(Event& event)
    {
        EventDispatcher ed(event);
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));
        ed.dispatch<WindowFocusLostEvent>(DRK_BIND_FN(on_window_focus_lost));
        ed.dispatch<MonitorEvent>(DRK_BIND_FN(on_monitor_event));
    }

    bool Game::on_key_pressed(const KeyPressedEvent& event)
    {
        switch (event.key)
        {
            case KeyCode::Space:
            {
                color = { randomDist(), randomDist(), randomDist() };
                DRK_LOG_GAME_INFO("Changed color to: ({}, {} {})", color.r, color.g, color.b);
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
            case KeyCode::Escape:
            {
                pause();
                return true;
            }
            case KeyCode::F11:
            {
                if (!m_debugLayer->is_layer_active())
                    m_debugLayer->activate_layer();
                else
                    m_debugLayer->deactivate_layer();
                return true;
            }
            default:
                return false;
        }
    }

    bool Game::on_window_focus_lost(const WindowFocusLostEvent& event)
    {
        pause();
        return true;
    }

    bool Game::on_monitor_event(const MonitorEvent& event)
    {
        pause();
        return false;
    }

    void Game::pause(void)
    {
        DRK_LOG_GAME_INFO("Paused");

        m_paused = true;
        deactivate_layer();
        open_pause_menu();
    }

    void Game::unpause(void)
    {
        DRK_LOG_GAME_INFO("Unpaused");

        m_paused = false;
        activate_layer();
    }

    bool Game::is_paused(void) const
    {
        return m_paused;
    }

    void Game::open_pause_menu(void)
    {
        auto pauseMenu = Layer::create<PauseMenu>();
        pauseMenu->set_unpause_callback_fn(DRK_BIND_FN(unpause));
        pauseMenu->set_exit_game_callback_fn([&]
        {
            detach_layer();
        });
        pauseMenu->set_save_game_callback_fn(DRK_BIND_FN(save));
        Application::get_instance().add_layer(pauseMenu);
    }

    void Game::save(void)
    {
        DRK_LOG_GAME_INFO("Saving Game");
    }
}
