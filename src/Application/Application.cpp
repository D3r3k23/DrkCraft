#include "Application.hpp"

#include "Graphics/Renderer.hpp"
// #include "Game/Game.hpp"
// #include "Engine/Timestep.hpp"
#include "Core/Util.hpp"

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

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
            DRK_LOG_ERROR("Application stopped with error code {}", status);
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

    const Window& Application::get_window(void)
    {
        return *(get_instance().m_window);
    }

    Application::Application(void)
      : m_running(false),
        m_minimized(false),
        m_exitCode(0)
    {
        DRK_LOG_TRACE("Initializing GLFW");
        auto status = glfwInit();
        DRK_ASSERT(status == GLFW_TRUE, "Failed to initialize GLFW");

        m_window = make_ptr<Window>("DrkCraft", 1280, 720);

        Renderer::init();

        // m_layerStack.push_front(m_imGuiLayer);
    }

    int Application::run(void)
    {
        // Window::Size viewPortSize = m_window->get_framebuffer_size();
        // glViewport(0, 0, viewPortSize.width, viewPortSize.height);

        std::array<float, 9> vertexPositions{
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glm::vec3 color{0.5f, 0.5f, 0.5f};
        RandomFloatDist dist(0.0f, 1.0f);

        bool space = false;
        glfwSetWindowUserPointer(m_window->get_native_window(), &space);
        glfwSetKeyCallback(m_window->get_native_window(), [](GLFWwindow* window, int key, int scanCode, int action, int mods){
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

                Renderer::begin();

                Renderer::draw_triangle(color, vertexArrayObject);

                Renderer::end();
            }

            glfwPollEvents();
            if (space)
            {
                color = { dist(), dist(), dist() };
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
        DRK_LOG_TRACE("Terminating GLFW");
        glfwTerminate();
    }
}
