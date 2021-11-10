#include "Application.hpp"

#include "Events.hpp"
#include "Graphics/Renderer.hpp"

#include <GLFW/glfw3.h>

#include <vector>

namespace DrkCraft
{
    Application* Application::s_instance = nullptr;
    int Application::s_exitCode = 0;

    void Application::init(void)
    {
        DRK_LOG_TRACE("Initializing Application");
        s_instance = new Application;
    }

    void Application::start(void)
    {
        DRK_LOG_TRACE("Starting application");
        s_exitCode = get_instance().run();

        if (s_exitCode)
            DRK_LOG_ERROR("Application stopped with error code {}", s_exitCode);
    }

    int Application::shutdown(void)
    {
        if (s_instance)
        {
            DRK_LOG_TRACE("Shutting down Application");
            delete s_instance;
            return s_exitCode;
        }
        else
            return 1;
    }

    Application& Application::get_instance(void)
    {
        DRK_ASSERT(s_instance, "Application not initialized");
        return *s_instance;
    }

    Application::Application(void)
      : m_running(false),
        m_minimized(false),
        m_layerStackView(m_layerStack),
        m_layerStackReverseView(m_layerStack),
        m_menuLayer(nullptr),
        m_gameLayer(nullptr)
    {
        DRK_LOG_TRACE("Initializing GLFW");
        init_glfw();

        DRK_LOG_TRACE("Creating Window");
        m_window = new Window("DrkCraft", 1280, 720);

        m_window->register_event_handler(DRK_BIND_EVENT_HANDLER(on_event));

        DRK_LOG_TRACE("Initializing Renderer");
        Renderer::init();

        open_main_menu();

        // ???
        // Window::Size viewPortSize = m_window->get_framebuffer_size();
        // glViewport(0, 0, viewPortSize.width, viewPortSize.height);
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

        glfwSetErrorCallback([](int error, const char* description)
        {
            DRK_LOG_ERROR("GLFW Error [{}]: {}", error, description);
        });
    }

    int Application::run(void)
    {
        m_running = true;
        while (m_running)
        {
            Timestep timestep;

            m_window->on_update();

            if (!m_minimized)
            {
                on_update(timestep);
                on_render(timestep);
            }
        }
        return 0;
    }

    void Application::on_update(Timestep timestep)
    {
        std::vector<Layer*> toRemove;
        for (Layer* layer : m_layerStackReverseView)
            if (layer->is_deleted())
                toRemove.push_back(layer);

        for (Layer* layer : toRemove)
            m_layerStack.remove(layer);

        for (Layer* layer : m_layerStackReverseView)
            if (layer->is_active())
                layer->on_update(timestep);
    }

    void Application::on_render(Timestep timestep)
    {
        Renderer::begin_frame();

        for (Layer* layer : m_layerStackReverseView)
            if (layer->is_active())
                layer->on_render(timestep);

        Renderer::end_frame();
    }

    bool Application::on_event(Event& event)
    {
        log_event(event);

        EventDispatcher ed(event);
        ed.dispatch<WindowCloseEvent>(DRK_BIND_EVENT_HANDLER(on_window_close));
        ed.dispatch<WindowResizeEvent>(DRK_BIND_EVENT_HANDLER(on_window_resize));

        for (Layer* layer : m_layerStackView)
            if (layer->is_active())
                layer->on_event(event);

        return true;
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

    void Application::open_main_menu(void)
    {
        DRK_ASSERT(!m_menuLayer, "MainMenu is already open");
        m_menuLayer = new MainMenu();
        m_layerStack.push_front(m_menuLayer);
    }

    void Application::close_main_menu(void)
    {
        bool removed = m_layerStack.remove(m_menuLayer);
        DRK_ASSERT(removed, "MainMenu is already closed");
    }

    void Application::start_game(void)
    {
        DRK_ASSERT(!m_gameLayer, "Game is already running");
        m_gameLayer = new Game();
        m_layerStack.push_front(m_gameLayer);
    }

    void Application::stop_game(void)
    {
        bool removed = m_layerStack.remove(m_gameLayer);
        DRK_ASSERT(removed, "Game is not running");
    }

    Window& Application::get_window(void)
    {
        DRK_ASSERT(m_window, "Window not initialized");
        return *m_window;
    }

    LayerStack& Application::get_layer_stack(void)
    {
        return m_layerStack;
    }
}
