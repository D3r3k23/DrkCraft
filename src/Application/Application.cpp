#include "Application.hpp"

#include "Graphics/Renderer.hpp"
#include "MainMenu.hpp"

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
        m_minimized(false)
    {
        DRK_LOG_TRACE("Initializing GLFW");
        init_glfw();

        DRK_LOG_TRACE("Creating Window");
        m_window = new Window("DrkCraft", 1280, 720);

        m_window->register_event_handler(DRK_BIND_FN(on_event));

        DRK_LOG_TRACE("Initializing Renderer");
        Renderer::init();

        // ???
        // Window::Size viewPortSize = m_window->get_framebuffer_size();
        // glViewport(0, 0, viewPortSize.width, viewPortSize.height);
    }

    Application::~Application(void)
    {
        DRK_LOG_TRACE("Clearing Layer Stack");
        m_layerStack.clear();

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

    void Application::add_overlay(const Ref<Layer>& layer)
    {
        layer->attach_layer();
        m_layerStack.push_front(layer);
    }

    void Application::add_layer(const Ref<Layer>& layer)
    {
        layer->attach_layer();
        m_layerStack.push_back(layer);
    }

    int Application::run(void)
    {
        add_layer(Layer::create<MainMenu>());

        m_running = true;
        while (m_running)
        {
            Timestep timestep;

            LayerStack frameLayerStack = m_layerStack;
            m_layerStackForwardView = make_ptr<LayerStack::ForwardView>(frameLayerStack);
            m_layerStackReverseView = make_ptr<LayerStack::ReverseView>(frameLayerStack);

            m_window->on_update();

            if (m_running && !m_minimized)
            {
                on_update(timestep);
                on_render(timestep);
            }

            m_layerStackForwardView.reset();
            m_layerStackReverseView.reset();

            m_layerStack.refresh();

            if (!m_layerStack.has_active_layer())
            {
                DRK_LOG_ERROR("There are still layers in the stack but none are active");
                DRK_LOG_INFO("Reactivating front layer");
                m_layerStack.activate_front();
            }

            if (m_layerStack.is_empty())
                m_running = false;
        }
        return on_exit();
    }

    int Application::on_exit(void)
    {
        return 0;
    }

    void Application::on_update(Timestep timestep)
    {
        for (auto& layer : *m_layerStackReverseView)
            if (layer->is_layer_active())
                layer->on_update(timestep);
    }

    void Application::on_render(Timestep timestep)
    {
        Renderer::begin_frame();
        // imgui::begin() ???

        for (auto& layer : *m_layerStackReverseView)
            if (layer->is_layer_active())
                layer->on_render(timestep);

        // imgui::end() ???
        Renderer::end_frame();
    }

    void Application::on_event(Event& event)
    {
        log_event(event);

        EventDispatcher ed(event);
        ed.dispatch<WindowCloseEvent>(DRK_BIND_FN(on_window_close));
        ed.dispatch<WindowResizeEvent>(DRK_BIND_FN(on_window_resize));

        for (auto& layer : *m_layerStackForwardView)
            if (layer->is_layer_active())
                layer->on_event(event);

        // if (event.handled())
        //     return true;
        // else
        // {
        //     Or just log here and specify if handled?
        //     DRK_LOG_WARN("Event: {} not handled", std::string(event));
        //     return false;
        // }
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

    Window& Application::get_window(void)
    {
        DRK_ASSERT(m_window, "Window not initialized");
        return *m_window;
    }
}
