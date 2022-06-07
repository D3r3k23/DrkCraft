#include "EventGenerator.hpp"

#include "System/Input.hpp"
#include "Core/Debug/Profiler.hpp"

#include <GLFW/glfw3.h>

#include <utility>

namespace DrkCraft
{
    namespace
    {
        void window_close_callback(GLFWwindow* window);
        void window_size_callback(GLFWwindow* window, int width, int height);
        void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        void window_pos_callback(GLFWwindow* window, int xPos, int yPos);
        void window_focus_callback(GLFWwindow* window, int focused);
        void window_maximize_callback(GLFWwindow* window, int maximized);
        void window_iconify_callback(GLFWwindow* window, int iconified);
        void window_content_scale_callback(GLFWwindow* window, float xScale, float yScale);
        void window_refresh_callback(GLFWwindow* window);

        void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods);
        void char_callback(GLFWwindow* window, uint codePoint);

        void cursor_pos_callback(GLFWwindow* window, double xPos, double yPos);
        void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
    };

    EventGenerator::EventGenerator(Window& window)
      : m_window(window)
    {
        DRK_LOG_CORE_TRACE("Creating EventGenerator");
    }

    EventGenerator::~EventGenerator(void)
    {
        DRK_LOG_CORE_TRACE("Destroying EventGenerator");

        GLFWwindow* window = m_window.get_raw_window();

        glfwSetWindowCloseCallback(window, nullptr);
        glfwSetWindowSizeCallback(window, nullptr);
        glfwSetFramebufferSizeCallback(window, nullptr);
        glfwSetWindowPosCallback(window, nullptr);
        glfwSetWindowFocusCallback(window, nullptr);
        glfwSetWindowMaximizeCallback(window, nullptr);
        glfwSetWindowIconifyCallback(window, nullptr);
        glfwSetWindowContentScaleCallback(window, nullptr);
        glfwSetWindowRefreshCallback(window, nullptr);

        glfwSetKeyCallback(window, nullptr);
        glfwSetCharCallback(window, nullptr);

        glfwSetCursorPosCallback(window, nullptr);
        glfwSetMouseButtonCallback(window, nullptr);
        glfwSetScrollCallback(window, nullptr);

        glfwSetWindowUserPointer(window, nullptr);
    }

    void EventGenerator::register_event_handler(AbstractEventHandlerFn<Event> handler)
    {
        m_handler = std::move(handler);

        glfwSetWindowUserPointer(m_window.get_raw_window(), static_cast<void*>(&m_handler));
        set_window_callbacks();
    }

    void EventGenerator::poll_events(void)
    {
        DRK_PROFILE_FUNCTION();
        glfwPollEvents();
    }

    void EventGenerator::set_window_callbacks(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Registering event callbacks with GLFW");

        GLFWwindow* window = m_window.get_raw_window();

        glfwSetWindowCloseCallback(window,        window_close_callback);
        glfwSetWindowSizeCallback(window,         window_size_callback);
        glfwSetFramebufferSizeCallback(window,    framebuffer_size_callback);
        glfwSetWindowPosCallback(window,          window_pos_callback);
        glfwSetWindowFocusCallback(window,        window_focus_callback);
        glfwSetWindowMaximizeCallback(window,     window_maximize_callback);
        glfwSetWindowIconifyCallback(window,      window_iconify_callback);
        glfwSetWindowContentScaleCallback(window, window_content_scale_callback);
        glfwSetWindowRefreshCallback(window,      window_refresh_callback);

        glfwSetKeyCallback(window,  key_callback);
        glfwSetCharCallback(window, char_callback);

        // glfwSetCursorPosCallback(window,   cursor_pos_callback); // Is this necessary?
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetScrollCallback(window,      scroll_callback);
    }

    namespace
    {
        void handle_event(GLFWwindow* window, Event& event)
        {
            const auto& handler = *static_cast<AbstractEventHandlerFn<Event>*>(glfwGetWindowUserPointer(window));
            {
                DRK_PROFILE_SCOPE(event.get_name());
                handler(event);
            }
            event.log_event();
        }

        // Window Events

        void window_close_callback(GLFWwindow* window)
        {
            WindowClosedEvent event;
            handle_event(window, event);
        }

        void window_size_callback(GLFWwindow* window, int width, int height)
        {
            WindowResizedEvent event((uint)width, (uint)height);
            handle_event(window, event);
        }

        void framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
            FramebufferResizedEvent event((uint)width, (uint)height);
            handle_event(window, event);
        }

        void window_pos_callback(GLFWwindow* window, int xPos, int yPos)
        {
            WindowMovedEvent event(xPos, yPos);
            handle_event(window, event);
        }

        void window_focus_callback(GLFWwindow* window, int focused)
        {
            if (focused == GLFW_TRUE)
            {
                WindowFocusGainedEvent event;
                handle_event(window, event);
            }
            else
            {
                WindowFocusLostEvent event;
                handle_event(window, event);
            }
        }

        void window_maximize_callback(GLFWwindow* window, int maximized)
        {
            if (maximized == GLFW_TRUE)
            {
                WindowMaximizedEvent event;
                handle_event(window, event);
            }
            else
            {
                WindowRestoredEvent event;
                handle_event(window, event);
            }
        }

        void window_iconify_callback(GLFWwindow* window, int iconified)
        {
            if (iconified == GLFW_TRUE)
            {
                WindowMinimizedEvent event;
                handle_event(window, event);
            }
            else
            {
                WindowRestoredEvent event;
                handle_event(window, event);
            }
        }

        void window_content_scale_callback(GLFWwindow* window, float xScale, float yScale)
        {
            WindowScaledEvent event(xScale, yScale);
            handle_event(window, event);
        }

        void window_refresh_callback(GLFWwindow* window)
        {
            WindowRefreshedEvent event;
            handle_event(window, event);
        }

        // Keyboard Events

        void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(to_key_code(key), to_key_mod_flags(mods));
                    handle_event(window, event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyHeldEvent event(to_key_code(key), to_key_mod_flags(mods));
                    handle_event(window, event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(to_key_code(key), to_key_mod_flags(mods));
                    handle_event(window, event);
                    break;
                }
                default:
                    DRK_ASSERT_DEBUG(false, "Unknown key action in callback");
            }
        }

        void char_callback(GLFWwindow* window, uint codePoint)
        {
            CharTypedEvent event(static_cast<char>(codePoint));
            handle_event(window, event);
        }

        // Mouse Events

        void cursor_pos_callback(GLFWwindow* window, double xPos, double yPos)
        {
            MouseMovedEvent event((float)xPos, (float)yPos);
            handle_event(window, event);
        }

        void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(to_mouse_code(button), to_key_mod_flags(mods));
                    handle_event(window, event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(to_mouse_code(button), to_key_mod_flags(mods));
                    handle_event(window, event);
                    break;
                }
                default:
                    DRK_ASSERT_DEBUG(false, "Unknown mouse button action in callback");
            }
        }

        void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
        {
            ScrollWheelMovedEvent event((float)xOffset, (float)yOffset);
            handle_event(window, event);
        }
    }
}
