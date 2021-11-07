#include "Application.hpp"

#include "Graphics/Renderer.hpp"
// #include "Game/Game.hpp"
// #include "Engine/Timestep.hpp"
#include "Core/Util.hpp"

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

        Renderer::init();

        // m_layerStack.push_front(m_imGuiLayer);
    }

    struct vec3 { double r, g, b; };

    int Application::run(void)
    {
        // Window::Size viewPortSize = m_window->get_framebuffer_size();
        // glViewport(0, 0, viewPortSize.width, viewPortSize.height);

        std::array<double, 9> vertexPositions{
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
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, nullptr);

        const char* vertexShaderText =
            "#version 400\n"
            "in vec3 vp;"
            "void main() {"
            "  gl_Position = vec4(vp, 1.0);"
            "}";

        const char* fragmentShaderText =
            "#version 400\n"
            "uniform vec4 color;"
            "out vec4 fragColor;"
            "void main() {"
            "  fragColor = color;"
            "}";

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderText, nullptr);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderText, nullptr);
        glCompileShader(fragmentShader);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, fragmentShader);
        glAttachShader(shaderProgram, vertexShader);
        glLinkProgram(shaderProgram);

        vec3 color{0.5, 0.0, 0.5};
        RandomDoubleDist dist(0.0, 1.0);

        bool space = false;
        glfwSetWindowUserPointer(m_window->get_window(), &space);
        glfwSetKeyCallback(m_window->get_window(), [](GLFWwindow* window, int key, int scanCode, int action, int mods){
            if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
                *(bool*)glfwGetWindowUserPointer(window) = true;
        });

        m_running = true;
        while (m_running)
        {
            // Timestep timestep;

            if (!m_minimized)
            {
                // for (auto& layer : m_layerStack)
                    // layer.on_update(timestep);

                // Render
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glUseProgram(shaderProgram);
                GLint colorUniformLocation = glGetUniformLocation(shaderProgram, "color");
                glUniform4f(colorUniformLocation, color.r, color.g, color.b, 1.0);

                glBindVertexArray(vertexArrayObject);
                glDrawArrays(GL_TRIANGLES, 0, 3);

                glfwSwapBuffers(m_window->get_window());
            }

            glfwPollEvents();
            if (space)
            {
                color = {dist(), dist(), dist()};
                space = false;
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
