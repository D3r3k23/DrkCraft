#include "EventGenerator.hpp"

namespace DrkCraft
{
    void EventGenerator::register_event_handler(GLFWwindow* window, const EventHandlerFn& handler)
    {
        m_window = window;
        m_handler = handler;

        glfwSetWindowUserPointer(m_window, static_cast<void*>(&m_handler));
    }

    void EventGenerator::register_event_callbacks(void)
    {
        using namespace GLFWEventCallbackFunctions;

        glfwSetWindowCloseCallback(m_window, window_close_callback);
        glfwSetWindowSizeCallback(m_window, window_size_callback);
        glfwSetWindowPosCallback(m_window, window_pos_callback);
        glfwSetWindowFocusCallback(m_window, window_focus_callback);
        glfwSetWindowIconifyCallback(m_window, window_iconify_callback);

        glfwSetKeyCallback(m_window, key_callback);
        glfwSetCharCallback(m_window, char_callback);

        glfwSetMouseButtonCallback(m_window, mouse_button_callback);
        glfwSetCursorPosCallback(m_window, cursor_pos_callback);
        glfwSetScrollCallback(m_window, scroll_callback);
    }

    // Calls m_eventHandler through GLFW user pointer
    #define DRK_CALL_EVENT_HANDLER_FN(event) (*static_cast<EventHandlerFn*>(glfwGetWindowUserPointer(window)))(event)

    namespace GLFWEventCallbackFunctions
    {
        ////////// Window Events //////////

        void window_close_callback(GLFWwindow* window)
        {
            WindowCloseEvent event;
            DRK_CALL_EVENT_HANDLER_FN(event);
        }

        void window_size_callback(GLFWwindow* window, int width, int height)
        {
            WindowResizeEvent event(width, height);
            DRK_CALL_EVENT_HANDLER_FN(event);
        }

        void window_pos_callback(GLFWwindow* window, int xPos, int yPos)
        {
            WindowMovedEvent event(xPos, yPos);
            DRK_CALL_EVENT_HANDLER_FN(event);
        }

        void window_focus_callback(GLFWwindow* window, int focused)
        {
            if (focused)
            {
                WindowFocusedEvent event;
                DRK_CALL_EVENT_HANDLER_FN(event);
            }
            else
            {
                WindowFocusLostEvent event;
                DRK_CALL_EVENT_HANDLER_FN(event);
            }
        }

        void window_iconify_callback(GLFWwindow* window, int iconified)
        {
            if (iconified)
            {
                WindowMinimizedEvent event;
                DRK_CALL_EVENT_HANDLER_FN(event);
            }
            else
            {
                WindowRestoredEvent event;
                DRK_CALL_EVENT_HANDLER_FN(event);
            }
        }

        ////////// Key Events //////////

        void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key);
                    DRK_CALL_EVENT_HANDLER_FN(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyHeldEvent event(key);
                    DRK_CALL_EVENT_HANDLER_FN(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    DRK_CALL_EVENT_HANDLER_FN(event);
                    break;
                }
                default:
                    DRK_ASSERT_FALSE("Unknown key action in callback");
            }
        }

        void char_callback(GLFWwindow* window, uint codePoint)
        {
            CharTypedEvent event(codePoint);
            DRK_CALL_EVENT_HANDLER_FN(event);
        }

        ////////// Mouse Events //////////

        void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    DRK_CALL_EVENT_HANDLER_FN(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    DRK_CALL_EVENT_HANDLER_FN(event);
                    break;
                }
                default:
                    DRK_ASSERT_FALSE("Unknown mouse button action in callback");
            }
        }

        void cursor_pos_callback(GLFWwindow* window, double xPos, double yPos)
        {
            MouseMovedEvent event(xPos, yPos);
            DRK_CALL_EVENT_HANDLER_FN(event);
        }

        void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
        {
            MouseScrolledEvent event(xOffset, yOffset);
            DRK_CALL_EVENT_HANDLER_FN(event);
        }
    }

    #undef DRK_CALL_EVENT_CALLBACK_FN
}
