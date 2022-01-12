#include "Application.hpp"

#include "Core/RunSettings.hpp"
#include "Graphics/Renderer.hpp"
#include "System/Input.hpp"
#include "Core/Profiler.hpp"

namespace DrkCraft
{
    Application* Application::s_instance = nullptr;

    void Application::init(void)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("Initializing GLFW");
        Window::init_glfw();

        s_instance = new Application;
    }

    int Application::shutdown(void)
    {
        DRK_PROFILE_FUNCTION();

        if (s_instance)
        {
            int exitCode = s_instance->m_exitCode;
            delete s_instance;

            DRK_LOG_CORE_TRACE("Shutting down GLFW");
            Window::shutdown_glfw();

            if (exitCode)
                DRK_LOG_CORE_ERROR("Application stopped with error code {}", exitCode);

            return exitCode;
        }
        else
        {
            DRK_LOG_CORE_WARN("Application is already shutdown");
            return 1;
        }
    }

    Application& Application::get_instance(void)
    {
        DRK_ASSERT_DEBUG(s_instance, "Application not initialized");
        return *s_instance;
    }

    Application::Application(void)
      : m_window("DrkCraft"),
        m_eventGenerator(m_window.get_raw_window()),
        m_imGuiManager(m_window.get_raw_window()),
        m_layerStackForwardView(m_frameLayerStack),
        m_layerStackReverseView(m_frameLayerStack),
        m_exitCode(0),
        m_running(false),
        m_minimized(false)
    {
        DRK_PROFILE_FUNCTION();

        if (RuntimeSettings::get().fullscreen)
            set_fullscreen();

        DRK_LOG_CORE_TRACE("Initializing Renderer");
        Renderer::init();

        const auto& viewportSize = m_window.get_framebuffer_size();
        Renderer::set_viewport(0, 0, viewportSize.x, viewportSize.y);

        DRK_LOG_CORE_TRACE("Registering Application event handler");
        m_eventGenerator.register_event_handler(DRK_BIND_FN(handle_event));
        m_monitorManager.register_event_handler(DRK_BIND_FN(handle_event));
    }

    Application::~Application(void)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("Clearing Layer Stack");
        m_frameLayerStack.clear();
        m_layerStack.clear();

        DRK_LOG_CORE_TRACE("Shutting down Renderer");
        Renderer::shutdown();
    }

    Window& Application::get_window(void)
    {
        return m_window;
    }

    MonitorManager& Application::get_monitors(void)
    {
        return m_monitorManager;
    }

    void Application::add_layer(const Ref<Layer>& layer)
    {
        layer->attach_layer();
        m_layerStack.push(layer);
    }

    void Application::add_overlay(const Ref<Layer>& layer)
    {
        layer->attach_layer();
        m_layerStack.push(layer, true);
    }

    void Application::run(void)
    {
        DRK_PROFILE_FUNCTION();

        m_running = true;
        while (m_running)
        {
            Timestep timestep;
            m_frameLayerStack = LayerStack::copy_active(m_layerStack);
            {
                DRK_PROFILE_SCOPE("Application core loop");
                m_window.poll_events();

                if (m_running && !m_minimized)
                {
                    update(timestep);
                    render();
                }
                m_window.swap_buffers();
            }

            m_layerStack.refresh();

            if (m_layerStack.empty())
            {
                DRK_LOG_CORE_INFO("LayerStack is empty; exiting Application");
                exit();
            }
            else if (m_layerStack.all_layers_inactive())
            {
                DRK_LOG_CORE_WARN("LayerStack is not empty, but there are no active layers");
                DRK_LOG_CORE_WARN("Reactivating back layer");
                m_layerStack.activate_back();
            }
        }
    }

    void Application::exit(int status)
    {
        m_running  = false;
        m_exitCode = status;
    }

    void Application::update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        for (auto& layer : m_layerStackReverseView)
            layer->on_update(timestep);
    }

    void Application::render(void)
    {
        DRK_PROFILE_FUNCTION();

        Renderer::begin_frame();
        m_imGuiManager.begin_frame();
        {
            DRK_PROFILE_SCOPE("Render Layers");
            for (auto& layer : m_layerStackReverseView)
                layer->on_render();
        }
        m_imGuiManager.end_frame();
        Renderer::end_frame();
    }

    void Application::handle_event(Event& event)
    {
        DRK_PROFILE_FUNCTION();

        EventDispatcher ed(event);
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));
        ed.dispatch<WindowClosedEvent>(DRK_BIND_FN(on_window_closed));
        ed.dispatch<FramebufferResizedEvent>(DRK_BIND_FN(on_framebuffer_resized));
        ed.dispatch<MonitorEvent>(DRK_BIND_FN(on_monitor_event));
        ed.dispatch<MonitorDisconnectedEvent>(DRK_BIND_FN(on_monitor_disconnected));

        // We could redraw the screen on resize/move

        m_imGuiManager.on_event(event);

        for (auto& layer : m_layerStackForwardView)
            layer->on_event(event);
    }

    bool Application::on_key_pressed(const KeyPressedEvent& event)
    {
        switch (event.key)
        {
        #if defined(DRK_CONFIG_DEBUG)
            case KeyCode::F12:
            {
                m_imGuiManager.toggle_demo_window();
                return true;
            }
        #endif
            default:
                return false;
        }
    }

    bool Application::on_window_closed(const WindowClosedEvent& event)
    {
        exit();
        return true;
    }

    bool Application::on_framebuffer_resized(const FramebufferResizedEvent& event)
    {
        Renderer::set_viewport(0, 0, event.width, event.height);
        return true;
    }

    bool Application::on_monitor_event(const MonitorEvent& event)
    {
        m_monitorManager.refresh_monitors();
        return false;
    }

    bool Application::on_monitor_disconnected(const MonitorDisconnectedEvent& event)
    {
        RuntimeSettings::get().fullscreen_monitor = 0;
        RuntimeSettings::save_settings();

        if (is_fullscreen())
            set_fullscreen();

        return false;
    }

    void Application::set_fullscreen(int monitor)
    {
        DRK_LOG_CORE_TRACE("Setting Application to fullscreen");

        if (monitor < 0)
            monitor = RuntimeSettings::get().fullscreen_monitor;

        m_monitorManager.activate_fullscreen(m_window, monitor);
    }

    void Application::set_windowed(void)
    {
        DRK_LOG_CORE_TRACE("Setting Application to windowed");

        m_monitorManager.deactivate_fullscreen(m_window);
    }

    bool Application::is_fullscreen(void) const
    {
        return m_monitorManager.fullscreen_activated();
    }

    bool Application::is_windowed(void) const
    {
        return !m_monitorManager.fullscreen_activated();
    }
}
