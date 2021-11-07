#ifndef DRK_APPLICATION_HPP
#define DRK_APPLICATION_HPP

#include "Core/Base.hpp"
#include "Window.hpp"
// #include "ImGuiLayer.hpp"

#include <deque>

namespace DrkCraft
{
    class Application
    {
    public:
        Application();
        int run(void);
        ~Application();

    private:
        Ptr<Window> m_window;
        // ImGuiLayer m_imGuiLayer;
        // std::deque<Layer&> m_layerStack;

        bool m_running;
        bool m_minimized;
    };
}

#endif // DRK_APPLICATION_HPP
