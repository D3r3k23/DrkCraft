#ifndef DRK_APPLICATION_HPP
#define DRK_APPLICATION_HPP

#include "Core/Base.hpp"
#include "Core/Timestep.hpp"
#include "Window.hpp"
#include "Layer.hpp"
// #include "ImGuiLayer.hpp"

#include <deque>

namespace DrkCraft
{
    class Application
    {
    public:
        static void init(void);
        static void start(void);
        static int  shutdown(void);

        static Application& get_instance(void);
        Window& get_window(void);

        Application();
        int run(void);
        ~Application();

        void init_glfw(void);

        void on_update(Timestep timestep);
        void on_render(Timestep timestep);
        void on_event(Event& event);

        bool on_window_close(WindowCloseEvent& event);
        bool on_window_resize(WindowResizeEvent& event);
        bool on_key_press(KeyPressedEvent& event);

    private:
        static Application* s_instance;

        Window* m_window;

        // ImGuiLayer m_imGuiLayer;
        // std::deque<Layer&> m_layerStack;

        // Layers: imGuiLayer -> debugLayer -> hudLayer -> (pauseMenuLayer) -> GameLayer

        bool m_running;
        bool m_minimized;

        int m_exitCode;
    };

    void glfw_error_callback(int error, const char* description);
}

#endif // DRK_APPLICATION_HPP
