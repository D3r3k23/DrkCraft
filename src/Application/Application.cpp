#include "Application.hpp"

#include "Events.hpp"
#include "Graphics/Renderer.hpp"
// #include "Game/Game.hpp"
#include "Core/Util.hpp"

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <array>

namespace DrkCraft
{
    Application* Application::s_instance = nullptr;

    void Application::init(void)
    {
        DRK_LOG_TRACE("Creating Application");
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
            DRK_LOG_TRACE("Shutting down Application");
            int status = s_instance->m_exitCode;
            delete s_instance;
            return status;
        }
        else
            return 1;
    }

    Application& Application::get_instance(void)
    {
        DRK_ASSERT(s_instance, "Application instance not initialized");
        return *s_instance;
    }

    Window& Application::get_window(void)
    {
        DRK_ASSERT(m_window, "Window not initialized");
        return *m_window;
    }

    static GLuint vertexArrayObject;
    static glm::vec3 color{0.5f, 0.5f, 0.5f};
    static RandomFloatDist dist(0.0f, 1.0f);

    Application::Application(void)
      : m_running(false),
        m_minimized(false),
        m_exitCode(0)
    {
        DRK_LOG_TRACE("Initializing GLFW");
        init_glfw();

        DRK_LOG_TRACE("Creating Window");
        m_window = new Window("DrkCraft", 1280, 720);

        m_window->register_event_handler(DRK_BIND_EVENT_FN(on_event));

        DRK_LOG_TRACE("Initializing Renderer");
        Renderer::init();

        // m_layerStack.push_front(m_imGuiLayer);


        // Window::Size viewPortSize = m_window->get_framebuffer_size();
        // glViewport(0, 0, viewPortSize.width, viewPortSize.height);

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
    }

    int Application::run(void)
    {
        m_running = true;
        while (m_running)
        {
            Timestep timestep;

            if (!m_minimized)
            {
                on_update(timestep);
                on_render(timestep);
            }

            m_window->on_update();
        }
        return 0;
    }

    Application::~Application(void)
    {
        DRK_LOG_TRACE("Shutting down Renderer");
        Renderer::shutdown();

        DRK_LOG_TRACE("Deleting Window");
        delete m_window;

        DRK_LOG_TRACE("Terminating GLFW");
        glfwTerminate();
    }

    void Application::init_glfw(void)
    {
        auto status = glfwInit();
        DRK_ASSERT(status == GLFW_TRUE, "Failed to initialize GLFW");

        glfwSetErrorCallback(glfw_error_callback);
    }

    void Application::on_update(Timestep timestep)
    {
        // for (auto& layer : m_layerStack)
            // layer.on_update(timestep);
    }

    void Application::on_render(Timestep timestep)
    {
        Renderer::begin();

        // for (auto& layer : m_layerStack)
            // layer.on_render();

        Renderer::draw_triangle(color, vertexArrayObject);

        Renderer::end();
    }

    void Application::on_event(Event& event)
    {
        if (event.get_type() != EventType::MouseMoved
         && event.get_type() != EventType::CharTyped
         && event.get_type() != EventType::KeyHeld)
            DRK_LOG_INFO("[Event] {}", std::string(event));

        EventDispatcher dispatcher(event);
        dispatcher.dispatch<WindowCloseEvent> (DRK_BIND_EVENT_FN(on_window_close));
        dispatcher.dispatch<WindowResizeEvent>(DRK_BIND_EVENT_FN(on_window_resize));

        dispatcher.dispatch<KeyPressedEvent>(DRK_BIND_EVENT_FN(on_key_press));

        // Or get rid of dispatcher and just check type?

        // for (auto& layer : m_layerStack)
            // if (event.handled)
                // break;
            // layer.on_event(event);
    }

    bool Application::on_window_close(WindowCloseEvent& event)
    {
        m_running = false;
        return true;
    }

    bool Application::on_window_resize(WindowResizeEvent& event)
    {
        return true;
    }

    bool Application::on_key_press(KeyPressedEvent& event)
    {
        if (event.key == KeyCode::Space)
        {
            color = { dist(), dist(), dist() };
            DRK_LOG_INFO("Changing triangle color to: ({}, {} {})", color.r, color.g, color.b);
            return true;
        }
        else
            return false;
    }

    void glfw_error_callback(int error, const char* description)
    {
        DRK_LOG_ERROR("GLFW Error [{}]: {}", error, description);
    }
}
