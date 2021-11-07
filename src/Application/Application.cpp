#include "Application.hpp"

// #include "Game/Game.hpp"
// #include "Engine/Timestep.hpp"

#include <GLFW/glfw3.h>

#include <array>

namespace DrkCraft
{
    Application* Application::s_instance = nullptr;

    void Application::init(void)
    {
        DRK_LOG_TRACE("Creating application");
        s_instance = new Application;
    }

    void Application::start(void)
    {
        DRK_ASSERT(s_instance, "Application not initialized");

        DRK_LOG_TRACE("Starting application");
        s_instance->run();

        if (int status = s_instance->m_exitCode)
            DRK_LOG_CRITICAL("Application stopped with error code {}", status);
    }

    int Application::shutdown(void)
    {
        if (s_instance)
        {
            DRK_LOG_TRACE("Shutting down application");
            int status = s_instance->m_exitCode;
            delete s_instance;
            return status;
        }
        else
            return 1;
    }

    Application& Application::get_instance(void)
    {
        return *s_instance;
    }

    Application::Application(void)
      : m_running(false),
        m_minimized(false),
        m_exitCode(0)
    {
        auto status = glfwInit();
        DRK_ASSERT(status == GLFW_TRUE, "Failed to initialize GLFW");

        m_window = make_ptr<Window>("DrkCraft", 1280, 720);

        // m_layerStack.push_front(m_imGuiLayer);

        //// Renderer ////
        const auto* renderer = glGetString(GL_RENDERER);
        const auto* version  = glGetString(GL_VERSION);
        DRK_LOG_INFO("Renderer: {}", renderer);
        DRK_LOG_INFO("OpenGL version: {}", version);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    int Application::run(void)
    {
        // Window::Size viewPortSize = m_window->get_framebuffer_size();
        // glViewport(0, 0, viewPortSize.width, viewPortSize.height);

        std::array<double, 9> vertexPositions = {
             0.0,  0.5, 0.0,
             0.5, -0.5, 0.0,
            -0.5, -0.5, 0.0
        };

        GLuint vertexBufferObject;
        glGenBuffers(1, &vertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions.data(), GL_STATIC_DRAW);

        GLuint vertexArrayObject;
        glGenVertexArrays(1, &vertexArrayObject);
        glBindVertexArray(vertexArrayObject);
        glEnableVertexAttribArray(0); // First attribute
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);

        const char* vertexShaderText =
            "#version 400\n"
            "in vec3 vp;"
            "void main() {"
            "  gl_Position = vec4(vp, 1.0);"
            "}";

        const char* fragmentShaderText =
            "#version 400\n"
            "out vec4 fragColor;"
            "void main() {"
            "  fragColor = vec4(0.5, 0.0, 0.5, 1.0);"
            "}";

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderText, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderText, NULL);
        glCompileShader(fragmentShader);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, fragmentShader);
        glAttachShader(shaderProgram, vertexShader);
        glLinkProgram(shaderProgram);

        m_running = true;
        while (m_running)
        {
            // Timestep timestep;

            if (!m_minimized)
            {
                // for (auto& layer : m_layerStack)
                    // layer.on_update(timestep);

                glfwPollEvents();

                // Render
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glUseProgram(shaderProgram);
                glBindVertexArray(vertexArrayObject);

                glDrawArrays(GL_TRIANGLES, 0, 3);
                glfwSwapBuffers(m_window->get_window());
            }

            if (m_window->should_close())
                m_running = false;

            // m_window.on_update();
        }
        return 0;
    }

    Application::~Application(void)
    {
        glfwTerminate();
    }
}
