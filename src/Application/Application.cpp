#include "Application.hpp"

// #include "Game/Game.hpp"
// #include "Engine/Timestep.hpp"

#include <GLFW/glfw3.h>

namespace DrkCraft
{
    Application::Application(void)
      : m_running(false),
        m_minimized(false)
    {
        auto status = glfwInit();
        DRK_ASSERT(status == GLFW_TRUE, "Failed to initialize GLFW");

        m_window = make_ptr<Window>("DrkCraft", 1280, 720);

        // m_layerStack.push_front(m_imGuiLayer);
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
        }
        return 0;
    }

    Application::~Application(void)
    {
        glfwTerminate();
    }
}
