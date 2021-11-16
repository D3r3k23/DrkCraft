#ifndef DRK_APPLICATION_HPP
#define DRK_APPLICATION_HPP

#include "Core/Base.hpp"
#include "Window.hpp"
#include "Events.hpp"
#include "ImGuiTools.hpp"
#include "Layer.hpp"
#include "LayerStack.hpp"
#include "Timestep.hpp"

namespace DrkCraft
{
    class Application
    {
    public:
        static void init(void);
        static int shutdown(void);

        static Application& get_instance(void);

        Application();
        ~Application();

        void init_glfw(void);

        void add_overlay(const Ref<Layer>& layer); // Pushes layer to front
        void add_layer(const Ref<Layer>& layer);   // Pushes layer to back

        void run(void);
        void exit(int status=0);

        void on_update(Timestep timestep);
        void on_render(Timestep timestep);
        void on_event(Event& event);

        bool on_window_close(WindowClosedEvent& event);
        bool on_window_resize(WindowResizedEvent& event);
        bool on_framebuffer_resize(FramebufferResizedEvent& event);

        Window& get_window(void);

    private:
        static Application* s_instance;

        Window* m_window;

        ImGuiManager* m_imGuiManager;

        LayerStack m_layerStack;
        Ptr<LayerStack::ForwardView> m_layerStackForwardView;
        Ptr<LayerStack::ReverseView> m_layerStackReverseView;

        int  m_exitCode;
        bool m_running;
        bool m_minimized;
    };
}

#endif // DRK_APPLICATION_HPP
