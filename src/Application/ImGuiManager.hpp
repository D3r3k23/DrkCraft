#ifndef DRK_IM_GUI_MANAGER_HPP
#define DRK_IM_GUI_MANAGER_HPP

#include "Core/Base.hpp"
#include "Events.hpp"

#include <GLFW/glfw3.h>

namespace DrkCraft
{
    class ImGuiManager
    {
    public:
        ImGuiManager(GLFWwindow* window);
        ~ImGuiManager(void);

        void begin_frame(void);
        void end_frame(void);

        void on_event(Event& event);

        void block_events(bool block);

    private:
        bool m_blockEvents;
    };
}

#endif // DRK_IM_GUI_MANAGER_HPP
