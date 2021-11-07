#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Base.hpp"
#include "Window.hpp"
#include "ImGuiLayer.hpp"

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
        void start(void);
        void stop(void);

    private:
        Window* m_window;
        ImGuiLayer m_imGuiLayer;
        std::deque<Layer&> m_layerStack;

        bool m_running;
        bool m_minimized;
    };
}

#endif // APPLICATION_HPP
