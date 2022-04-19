#include "Application.hpp"

#include "System/GlfwTools.hpp"
#include "Graphics/Renderer/Renderer.hpp"
#include "Audio/Audio.hpp"
#include "System/Icon.hpp"
#include "System/Input.hpp"
#include "Core/Settings.hpp"
#include "Application/Layers/MainMenu.hpp"
#include "Game/Layers/GameLayer.hpp"
#include "Util/Time.hpp"
#include "Core/Debug/Profiler.hpp"

#include <thread>

namespace DrkCraft
{
    ////////////////////////
    //       Static       //
    ////////////////////////

    std::optional<Application> Application::s_instance;

    void Application::init(std::string_view title)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("Initializing GLFW");
        init_glfw();

        s_instance.emplace(title);
    }

    int Application::shutdown(void)
    {
        DRK_PROFILE_FUNCTION();

        if (s_instance)
        {
            int exitCode = s_instance->m_exitCode;
            s_instance.reset();

            DRK_LOG_CORE_TRACE("Shutting down GLFW");
            shutdown_glfw();

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

    void Application::run(void)
    {
        get_instance().run_internal();
    }

    void Application::exit(int status)
    {
        get_instance().exit_internal(status);
    }

    void Application::add_layer(const Ref<Layer>& layer)
    {
        DRK_PROFILE_FUNCTION();

        get_instance().m_layerStack.push(layer);
        layer->attach_layer();
    }

    void Application::add_overlay(const Ref<Layer>& layer)
    {
        DRK_PROFILE_FUNCTION();

        get_instance().m_layerStack.push(layer, true);
        layer->attach_layer();
    }

    void Application::post_event(Event& event)
    {
        get_instance().handle_event(event);
    }

    Window& Application::get_window(void)
    {
        return get_instance().m_window;
    }

    MonitorManager& Application::get_monitors(void)
    {
        return get_instance().m_monitorManager;
    }

    AssetLibrary& Application::get_asset_library(void)
    {
        return get_instance().m_assetLibrary;
    }

    ImGuiController& Application::get_imgui(void)
    {
        return *(get_instance().m_imGuiController);
    }

    OpenGlContext& Application::get_gl_context(void)
    {
        return get_instance().m_context;
    }

    //////////////////////////
    //       Instance       //
    //////////////////////////

    Application::Application(std::string_view title)
      : m_window(title),
        m_context(m_window),
        m_eventGenerator(m_window),
        m_layerStackForwardView(m_frameLayerStack),
        m_layerStackReverseView(m_frameLayerStack),
        m_exitCode(0),
        m_running(false),
        m_minimized(false)
    {
        DRK_PROFILE_FUNCTION();

        const auto& settings = RuntimeSettings::get_settings();
        {
            DRK_LOG_CORE_TRACE("Loading monitors");
            DRK_PROFILE_THREAD_CREATE("monitor_load");
            // Probably should eventually do this on the main thread,
            // since GLFW calls may not be thread safe
            std::jthread monitorLoadThread([this]()
            {
                DRK_PROFILE_THREAD("monitor_load");
                m_monitorManager.load_monitors();
            });

            // Set up logo splash screen

            DRK_LOG_CORE_TRACE("Setting window icon");
            m_window.set_icon(Icon(icon_asset_path("icon.png")));

            DRK_LOG_CORE_TRACE("Initializing Audio system");
            Audio::init(settings.audio.volume);

            DRK_LOG_CORE_TRACE("Initializing Renderer");
            Renderer::init(m_context, m_window.get_framebuffer_size());

            DRK_LOG_CORE_TRACE("Initializing ImGui");
            m_imGuiController.emplace(m_window);

            m_window.set_vsync(settings.video.vsync);

            DRK_LOG_CORE_TRACE("Loading assets");
            load_assets();
        }
        if (settings.video.fullscreen)
            set_fullscreen();

        DRK_LOG_CORE_TRACE("Registering Application event handler");
        m_eventGenerator.register_event_handler(DRK_BIND_FN(handle_event));
        m_monitorManager.register_event_handler(DRK_BIND_FN(handle_event));

        DRK_LOG_CORE_INFO("Application initialized");
    }

    Application::~Application(void)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("Clearing Layer Stack");
        m_frameLayerStack.clear();
        m_layerStack.clear();

        m_assetLibrary.unload_all();

        DRK_LOG_CORE_TRACE("Saving Settings");
        RuntimeSettings::save();

        DRK_LOG_CORE_TRACE("Shutting down Renderer");
        Renderer::shutdown();

        m_imGuiController.reset();

        DRK_LOG_CORE_TRACE("Shutting down Audio system");
        Audio::shutdown();
    }

    void Application::run_internal(void)
    {
        DRK_PROFILE_FUNCTION();

        const auto startupTime = Time::as_duration<Time::Seconds<>>(Time::get_program_time());
        DRK_LOG_CORE_INFO("Startup time: {:.3f}", startupTime.count());

        TimestepGenerator timestepGen;

        m_running = true;
        while (m_running)
        {
            DRK_PROFILE_EVENT("New frame");

            Timestep timestep = timestepGen.get_timestep();
            m_frameLayerStack = LayerStack::copy_active(m_layerStack);
            {
                DRK_PROFILE_SCOPE("Application core loop");
                m_eventGenerator.poll_events();

                if (m_running && !m_minimized)
                {
                    update(timestep);
                    render();
                }
                m_context.swap_buffers();
            }
            m_layerStack.refresh();

            if (m_layerStack.empty())
            {
                DRK_LOG_CORE_INFO("LayerStack is empty; exiting Application");
                exit_internal(0);
            }
            else if (m_layerStack.all_layers_inactive())
            {
                DRK_LOG_CORE_WARN("LayerStack is not empty, but there are no active layers");
                DRK_LOG_CORE_WARN("Reactivating back layer");
                m_layerStack.activate_back();
            }
        }
    }

    void Application::exit_internal(int status)
    {
        if (!m_running)
            DRK_LOG_CORE_WARN("Application is not running!");

        m_running  = false;
        m_exitCode = status;
    }

    void Application::render(void)
    {
        DRK_PROFILE_FUNCTION();

        Renderer::begin_frame();
        m_imGuiController->begin_frame();
        {
            DRK_PROFILE_SCOPE("Render Layers");
            for (auto& layer : m_layerStackReverseView)
                layer->on_render();
        }
        m_imGuiController->end_frame();
        Renderer::end_frame();
    }

    void Application::update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        for (auto& layer : m_layerStackReverseView)
            layer->on_update(timestep);
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

        if (event == EventCategory::Input)
            m_imGuiController->on_event(event_cast<InputEvent>(event));

        for (auto& layer : m_layerStackForwardView)
            layer->on_event(event);
    }

    bool Application::on_key_pressed(const KeyPressedEvent& event)
    {
        switch (event.key)
        {
            case KeyCode::F7:
            {
                if constexpr (DRK_CONFIG_DEBUG)
                {
                    m_imGuiController->toggle_demo_window();
                    return true;
                }
                else
                    return false;
            }
            default:
                return false;
        }
    }

    bool Application::on_window_closed(const WindowClosedEvent& event)
    {
        exit_internal(0);
        return true;
    }

    bool Application::on_framebuffer_resized(const FramebufferResizedEvent& event)
    {
        Renderer::set_viewport(0, 0, event.width, event.height);
        return true;
    }

    bool Application::on_monitor_event(const MonitorEvent& event)
    {
        m_monitorManager.refresh_monitors(); // Will this work?
        return false;
    }

    bool Application::on_monitor_disconnected(const MonitorDisconnectedEvent& event)
    {
        RuntimeSettings::settings().video.fs_monitor = 0;

        if (is_fullscreen())
            set_fullscreen(0);

        return false;
    }

    void Application::load_assets(void)
    {
        DRK_PROFILE_FUNCTION();

        m_assetLibrary.load_list(MainMenu::get_asset_list());
        m_assetLibrary.load_list(Game::get_asset_list());
    }

    void Application::set_fullscreen(int monitor)
    {
        DRK_LOG_CORE_TRACE("Setting Application to fullscreen");

        if (monitor < 0)
            monitor = RuntimeSettings::get_settings().video.fs_monitor;

        m_monitorManager.activate_fullscreen(m_window, monitor);
        DRK_LOG_CORE_INFO("Fullscreen monitor: {}", m_monitorManager.get_monitor(monitor).get_name());
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
