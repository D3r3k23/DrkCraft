#ifndef DRK_APPLICATION_HPP
#define DRK_APPLICATION_HPP

#include "Core/Base.hpp"
#include "System/Window.hpp"
#include "System/Monitor.hpp"
#include "System/AssetManager.hpp"
#include "Events.hpp"
#include "EventGenerator.hpp"
#include "ImGuiTools.hpp"
#include "Layer.hpp"
#include "LayerStack.hpp"
#include "Core/Timestep.hpp"
#include "Graphics/Util.hpp"

namespace DrkCraft
{
    class Application
    {
    public:
        static void init(void);
        static int shutdown(void);

        static Application& get_instance(void);
        static Window& get_window(void);
        static MonitorManager& get_monitors(void);
        static AssetManager& get_assets(void);

        Application();
        ~Application();

        void add_layer(const Ref<Layer>& layer);
        void add_overlay(const Ref<Layer>& layer);

        void run(void);
        void exit(int status=0);

    private:
        void update(Timestep timestep);
        void render(void);
        void handle_event(Event& event);

        bool on_key_pressed(const KeyPressedEvent& event);
        bool on_window_closed(const WindowClosedEvent& event);
        bool on_framebuffer_resized(const FramebufferResizedEvent& event);

        bool on_monitor_event(const MonitorEvent& event);
        bool on_monitor_disconnected(const MonitorDisconnectedEvent& event);

    public:
        void set_fullscreen(int monitor=-1);
        void set_windowed(void);
        bool is_fullscreen(void) const;
        bool is_windowed(void) const;

    private:
        static Application* s_instance;

        Window m_window;
        OpenGlLoader m_openGlLoader;

        EventGenerator m_eventGenerator;
        MonitorManager m_monitorManager;
        AssetManager m_assetManager;
        ImGuiManager m_imGuiManager;

        LayerStack m_layerStack;
        LayerStack m_frameLayerStack;
        LayerStack::ForwardView m_layerStackForwardView;
        LayerStack::ReverseView m_layerStackReverseView;

        int  m_exitCode;
        bool m_running;
        bool m_minimized;
    };
}

#endif // DRK_APPLICATION_HPP
