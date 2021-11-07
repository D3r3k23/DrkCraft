#include "Application.hpp"

// #include "Game/Game.hpp"
// #include "Engine/Timestep.hpp"

namespace DrkCraft
{
    Application::Application(void)
    {
        // m_layerStack.push_front(m_imGuiLayer);

        m_running = false;
        m_minimized = false;
    }

    int Application::run(void)
    {
        m_running = true;
        while (m_running)
        {
            // Timestep timestep;

            if (!m_minimized)
            {
                // for (auto& layer : m_layerStack)
                    // layer.on_update(timestep);


            }

            // m_window.on_update();
            break;
        }
        return 0;
    }

    Application::~Application(void)
    {

    }
}
