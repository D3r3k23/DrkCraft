#include "Game.hpp"

#include "Application/Application.hpp"
#include "Graphics/Renderer/Renderer.hpp"
#include "Audio/Audio.hpp"
#include "System/Input.hpp"
#include "Game/World/ChunkRenderer.hpp"
#include "Core/Debug/Profiler.hpp"

#include <utility>

// Temp
#include <array>
#include <glad/glad.h>

namespace DrkCraft
{
    static const AssetList s_REQUIRED_ASSETS
    {
        { AssetType::Texture, "blockatlas.png" },
        { AssetType::Texture, "skybox.png"     },

        { AssetType::Song, "Alix Perez - Burning Babylon.mp3" }
    };

    const AssetList& Game::get_asset_list(void)
    {
        return s_REQUIRED_ASSETS;
    }

    Game::Game(Ptr<World> world, AssetManager& assets)
      : m_assets(assets),
        m_world(move(world)),
        m_running(true),
        m_paused(false),
        flatColorShaderProgram("FlatColorShaderProgram"),
        color(0.5f, 0.5f, 0.5f),
        randomDist(0.0f, 1.0f)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_GAME_INFO("Starting game");

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

        flatColorShaderProgram.attach(Shader::create(
            shader_asset_path("flat_color_vertex_shader.glsl"), ShaderType::Vertex));
        flatColorShaderProgram.attach(Shader::create(
            shader_asset_path("flat_color_fragment_shader.glsl"), ShaderType::Fragment));
        flatColorShaderProgram.link();

        song = m_assets.get_song("Alix Perez - Burning Babylon.mp3");
    }

    Game::~Game(void)
    {

    }

    void Game::render(void)
    {
        DRK_PROFILE_FUNCTION();

        Renderer::begin_scene({
            m_player.get_camera()
        });

        m_world->skybox.render();

        m_chunkRenderer.render(m_world->chunkManager);
        m_player.render();

        Renderer::end_scene();


        flatColorShaderProgram.bind();
        // flatColorShaderProgram.upload_uniform("u_viewProjection", m_player.get_view_projection());
        // flatColorShaderProgram.upload_uniform("u_transform", Transform::Identity());
        flatColorShaderProgram.upload_uniform("u_color", vec4(color, 1.0f));

        Renderer::draw_triangle(*vertexBuffer);

        flatColorShaderProgram.unbind();
    }

    void Game::update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        m_player.update(timestep);
    }

    void Game::on_event(InputEvent& event)
    {
        EventDispatcher ed(event);
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));

        m_player.on_event(event);
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

    const Player& Game::get_player(void) const
    {
        return m_player;
    }

    void Game::save(void)
    {
        DRK_LOG_GAME_INFO("Saving Game");
    }
}
