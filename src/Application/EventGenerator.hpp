#ifndef DRK_EVENT_GENERATOR_HPP
#define DRK_EVENT_GENERATOR_HPP

#include "Core/Base.hpp"
#include "Events.hpp"

#include <GLFW/glfw3.h>

namespace DrkCraft
{
    class EventGenerator
    {
    public:
        EventGenerator(GLFWwindow* window);
        void register_event_handler(const AbstractEventHandlerFn& handler);
        void register_event_callbacks(void);

    private:
        GLFWwindow* m_window;
        AbstractEventHandlerFn m_handler;
    };

    namespace GLFWEventCallbackFunctions
    {
        void window_close_callback(GLFWwindow* window);
        void window_size_callback(GLFWwindow* window, int width, int height);
        void window_pos_callback(GLFWwindow* window, int xPos, int yPos);
        void window_focus_callback(GLFWwindow* window, int focused);
        void window_iconify_callback(GLFWwindow* window, int iconified);

        void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods);
        void char_callback(GLFWwindow* window, uint codePoint);

        void cursor_pos_callback(GLFWwindow* window, double xPos, double yPos);
        void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
    }
}

#endif // DRK_EVENT_GENERATOR_HPP
