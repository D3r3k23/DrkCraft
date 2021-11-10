#ifndef DRK_APPLICATION_HPP
#define DRK_APPLICATION_HPP

#include "Core/Base.hpp"
#include "Window.hpp"
#include "Events.hpp"
#include "Layer.hpp"
#include "LayerStack.hpp"
#include "Timestep.hpp"

namespace DrkCraft
{
    class Application
    {
    public:
        static void init(void);
        static void start(void);
        static int  shutdown(void);

        static Application& get_instance(void);

        Application();
        ~Application();

        void init_glfw(void);

        void add_overlay(const Ref<Layer>& layer);
        void add_layer(const Ref<Layer>& layer);

        int run(void);
        int on_exit(void);

        void on_update(Timestep timestep);
        void on_render(Timestep timestep);
        void on_event(Event& event);

        bool on_window_close(WindowCloseEvent& event);
        bool on_window_resize(WindowResizeEvent& event);
        bool on_key_press(KeyPressedEvent& event);

        Window& get_window(void);

    private:
        static Application* s_instance;
        static int s_exitCode;

        Window* m_window;

        LayerStack m_layerStack;
        Ptr<LayerStack::ForwardView> m_layerStackForwardView;
        Ptr<LayerStack::ReverseView> m_layerStackReverseView;

        bool m_running;
        bool m_minimized;
    };
}

#endif // DRK_APPLICATION_HPP
