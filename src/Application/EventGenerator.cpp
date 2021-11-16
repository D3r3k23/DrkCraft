#include "EventGenerator.hpp"

#include "Input.hpp"
#include "Core/Profiler.hpp"

namespace DrkCraft
{
    EventGenerator::EventGenerator(GLFWwindow* window)
      : m_window(window)
    { }

    void EventGenerator::register_window_event_handler(const AbstractEventHandlerFn& handler)
    {
        m_handler = handler;
        glfwSetWindowUserPointer(m_window, static_cast<void*>(&m_handler));
        set_window_callbacks();
    }

    void EventGenerator::set_window_callbacks(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Registering event callbacks with GLFW");

        using namespace WindowEventCallbackFunctions;

        glfwSetWindowCloseCallback(m_window, window_close_callback);
        glfwSetWindowSizeCallback(m_window, window_size_callback);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        glfwSetWindowPosCallback(m_window, window_pos_callback);
        glfwSetWindowFocusCallback(m_window, window_focus_callback);
        glfwSetWindowMaximizeCallback(m_window, window_maximize_callback);
        glfwSetWindowIconifyCallback(m_window, window_iconify_callback);
        glfwSetWindowContentScaleCallback(m_window, window_content_scale_callback);
        glfwSetWindowRefreshCallback(m_window, window_refresh_callback);

        glfwSetKeyCallback(m_window, key_callback);
        glfwSetCharCallback(m_window, char_callback);

        glfwSetCursorPosCallback(m_window, cursor_pos_callback);
        glfwSetMouseButtonCallback(m_window, mouse_button_callback);
        glfwSetScrollCallback(m_window, scroll_callback);
    }

    // Calls m_eventHandler through GLFW user pointer
    #define DRK_CALL_EVENT_HANDLER_FN(event) \
        (*static_cast<AbstractEventHandlerFn*>(glfwGetWindowUserPointer(window)))(event)

    namespace WindowEventCallbackFunctions
    {
        ////////// Window Events //////////

        void window_close_callback(GLFWwindow* window)
        {
            WindowClosedEvent event;
            DRK_CALL_EVENT_HANDLER_FN(event);
        }

        void window_size_callback(GLFWwindow* window, int width, int height)
        {
            WindowResizedEvent event((uint)width, (uint)height);
            DRK_CALL_EVENT_HANDLER_FN(event);
        }

        void framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
            FramebufferResizedEvent event((uint)width, (uint)height);
            DRK_CALL_EVENT_HANDLER_FN(event);
        }

        void window_pos_callback(GLFWwindow* window, int xPos, int yPos)
        {
            WindowMovedEvent event(xPos, yPos);
            DRK_CALL_EVENT_HANDLER_FN(event);
        }

        void window_focus_callback(GLFWwindow* window, int focused)
        {
            if (focused == GLFW_TRUE)
            {
                WindowFocusGainedEvent event;
                DRK_CALL_EVENT_HANDLER_FN(event);
            }
            else
            {
                WindowFocusLostEvent event;
                DRK_CALL_EVENT_HANDLER_FN(event);
            }
        }

        void window_maximize_callback(GLFWwindow* window, int maximized)
        {
            if (maximized == GLFW_TRUE)
            {
                WindowMaximizedEvent event;
                DRK_CALL_EVENT_HANDLER_FN(event);
            }
            else
            {
                WindowRestoredEvent event;
                DRK_CALL_EVENT_HANDLER_FN(event);
            }
        }

        void window_iconify_callback(GLFWwindow* window, int iconified)
        {
            if (iconified == GLFW_TRUE)
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

        void window_content_scale_callback(GLFWwindow* window, float xScale, float yScale)
        {
            WindowScaledEvent event(xScale, yScale);
            DRK_CALL_EVENT_HANDLER_FN(event);
        }

        void window_refresh_callback(GLFWwindow* window)
        {
            WindowRefreshedEvent event;
            DRK_CALL_EVENT_HANDLER_FN(event);
        }

        ////////// Keyboard Events //////////

        void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(to_key_code(key), get_input_mod_flags(mods));
                    DRK_CALL_EVENT_HANDLER_FN(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyHeldEvent event(to_key_code(key), get_input_mod_flags(mods));
                    DRK_CALL_EVENT_HANDLER_FN(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(to_key_code(key), get_input_mod_flags(mods));
                    DRK_CALL_EVENT_HANDLER_FN(event);
                    break;
                }
                default:
                    DRK_ASSERT_DEBUG(false, "Unknown key action in callback");
            }
        }

        void char_callback(GLFWwindow* window, uint codePoint)
        {
            CharTypedEvent event(static_cast<char>(codePoint));
            DRK_CALL_EVENT_HANDLER_FN(event);
        }

        ////////// Mouse Events //////////

        void cursor_pos_callback(GLFWwindow* window, double xPos, double yPos)
        {
            MouseMovedEvent event((float)xPos, (float)yPos);
            DRK_CALL_EVENT_HANDLER_FN(event);
        }

        void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(to_mouse_code(button), get_input_mod_flags(mods));
                    DRK_CALL_EVENT_HANDLER_FN(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(to_mouse_code(button), get_input_mod_flags(mods));
                    DRK_CALL_EVENT_HANDLER_FN(event);
                    break;
                }
                default:
                    DRK_ASSERT_DEBUG(false, "Unknown mouse button action in callback");
            }
        }

        void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
        {
            ScrollWheelMovedEvent event((float)xOffset, (float)yOffset);
            DRK_CALL_EVENT_HANDLER_FN(event);
        }
    }
}
