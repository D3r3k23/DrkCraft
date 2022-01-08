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
        ~EventGenerator(void);

        void register_window_event_handler(const AbstractEventHandlerFn& handler);

    private:
        void set_window_callbacks(void);

        static void handle_event(GLFWwindow* window, Event& event);

        class Callbacks
        {
        public:
            static void window_close_callback(GLFWwindow* window);
            static void window_size_callback(GLFWwindow* window, int width, int height);
            static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
            static void window_pos_callback(GLFWwindow* window, int xPos, int yPos);
            static void window_focus_callback(GLFWwindow* window, int focused);
            static void window_maximize_callback(GLFWwindow* window, int maximized);
            static void window_iconify_callback(GLFWwindow* window, int iconified);
            static void window_content_scale_callback(GLFWwindow* window, float xScale, float yScale);
            static void window_refresh_callback(GLFWwindow* window);

            static void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods);
            static void char_callback(GLFWwindow* window, uint codePoint);

            static void cursor_pos_callback(GLFWwindow* window, double xPos, double yPos);
            static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
            static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
        };

    private:
        GLFWwindow* m_window;
        AbstractEventHandlerFn m_handler;
    };
}

#endif // DRK_EVENT_GENERATOR_HPP
