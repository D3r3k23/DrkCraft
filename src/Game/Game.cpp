#include "Game.hpp"

#include "Application/Application.hpp"
#include "PauseMenu.hpp"

namespace DrkCraft
{
    Game::Game(void)
      : Layer("GameLayer"),
        flatColorShaderProgram("FlatColorShaderProgram"),
        color(0.5f, 0.5f, 0.5f),
        randomDist(0.0f, 1.0f)
    {
        std::array<float, 9> vertexPositions{
             0.0f,  0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
        };

        GLuint vertexBufferObject;
        glGenBuffers(1, &vertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions.data(), GL_STATIC_DRAW);

        glGenVertexArrays(1, &vertexArrayObject);
        glBindVertexArray(vertexArrayObject);
        glEnableVertexAttribArray(0); // First attribute
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        flatColorShaderProgram.add_shader(Shader::create("data/shaders/flat_color_vertex_shader.glsl", ShaderType::Vertex));
        flatColorShaderProgram.add_shader(Shader::create("data/shaders/flat_color_fragment_shader.glsl", ShaderType::Fragment));
        flatColorShaderProgram.link();
    }

    void Game::on_update(Timestep timestep)
    {

    }

    void Game::on_render(Timestep timestep)
    {
        flatColorShaderProgram.bind();
        flatColorShaderProgram.upload_uniform_float4("u_color", glm::vec4(color, 1.0f));
        Renderer::draw_triangle(vertexArrayObject);
        flatColorShaderProgram.unbind();
    }

    void Game::on_event(Event& event)
    {
        EventDispatcher ed(event);
        ed.dispatch<KeyPressedEvent>(DRK_BIND_EVENT_HANDLER(on_key_pressed));
    }

    bool Game::on_key_pressed(KeyPressedEvent& event)
    {
        switch (event.key)
        {
            case KeyCode::Space:
            {
                color = { randomDist(), randomDist(), randomDist() };
                DRK_LOG_INFO("Changing triangle color to: ({}, {} {})", color.r, color.g, color.b);
                return true;
            }
            case KeyCode::Escape: // Or pause button
            {
                // pause();
                // return true;
                return false;
            }
            default:
                return false;
        }
    }

    void Game::pause(void)
    {
        m_paused = true;
        Application::get_instance().get_layer_stack().push_front(new PauseMenu());
    }
}
