#ifndef DRK_APPLICATION_HPP
#define DRK_APPLICATION_HPP

#include "Core/Base.hpp"
#include "Window.hpp"
// #include "Layer.hpp"
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

        Application();
        int run(void);
        ~Application();

    private:
        static Application* s_instance;

        Ptr<Window> m_window;
        // ImGuiLayer m_imGuiLayer;
        // std::deque<Layer&> m_layerStack;

        bool m_running;
        bool m_minimized;

        int m_exitCode;
    };
}

#endif // DRK_APPLICATION_HPP
