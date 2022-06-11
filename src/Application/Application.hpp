#ifndef DRK_APPLICATION_APPLICATION_HPP
#define DRK_APPLICATION_APPLICATION_HPP

#include "Core/Base.hpp"
#include "Graphics/OpenGlContext.hpp"
#include "System/Window.hpp"
#include "System/Monitor.hpp"
#include "System/AssetLibrary.hpp"
#include "Application/ImGuiController.hpp"
#include "Application/Events.hpp"
#include "Application/EventGenerator.hpp"
#include "Application/Layer.hpp"
#include "Application/LayerStack.hpp"
#include "Util/Timestep.hpp"

#include <string_view>

namespace DrkCraft
{
    class Application
    {
    public:
        static void init(std::string_view title);
        static int shutdown(void);

        static Application& get_instance(void);

        static void run(void);
        static void exit(int status=0);

        static void add_layer(const Ref<Layer>& layer);
        static void add_overlay(const Ref<Layer>& layer);

        static void post_event(Event& event);

        static Window& get_window(void);
        static MonitorManager& get_monitors(void);
        static AssetLibrary& get_asset_library(void);
        static ImGuiController& get_imgui(void);
        static OpenGlContext& get_gl_context(void);

    public:
        Application(std::string_view title);
        ~Application(void);

    private:
        void run_internal(void);
        void exit_internal(int status);

        void render(void);
        void update(Timestep timestep);
        void handle_event(Event& event);

        bool on_key_pressed(const KeyPressedEvent& event);
        bool on_window_closed(const WindowClosedEvent& event);
        bool on_framebuffer_resized(const FramebufferResizedEvent& event);

        bool on_window_minimized(const WindowMinimizedEvent& event);
        bool on_window_restored(const WindowRestoredEvent& event);

        bool on_monitor_event(const MonitorEvent& event);
        bool on_monitor_disconnected(const MonitorDisconnectedEvent& event);

        void load_assets(void);

    public:
        void set_fullscreen(int monitor=-1);
        void set_windowed(void);
        bool is_fullscreen(void) const;
        bool is_windowed(void) const;
        bool is_minimized(void) const;

    private:
        static Optional<Application> s_instance;

        Window m_window;
        OpenGlContext m_context;

        MonitorManager m_monitorManager;
        AssetLibrary  m_assetLibrary;

        Optional<ImGuiController> m_imGuiController;
        EventGenerator m_eventGenerator;

        LayerStack m_layerStack;
        LayerStack m_frameLayerStack;
        LayerStack::ForwardView m_layerStackForwardView;
        LayerStack::ReverseView m_layerStackReverseView;

        int  m_exitCode;
        bool m_running;
        bool m_minimized;
    };
}

#endif // DRK_APPLICATION_APPLICATION_HPP
