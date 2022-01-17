#include "EventGenerator.hpp"

#include "System/Input.hpp"
#include "Core/Profiler.hpp"

namespace DrkCraft
{
    EventGenerator::EventGenerator(Window& window)
      : m_window(window.get_raw_window())
    {
        DRK_LOG_CORE_TRACE("Creating EventGenerator");
    }

    EventGenerator::~EventGenerator(void)
    {
        DRK_LOG_CORE_TRACE("Destroying EventGenerator");

        glfwSetWindowCloseCallback(m_window, nullptr);
        glfwSetWindowSizeCallback(m_window, nullptr);
        glfwSetFramebufferSizeCallback(m_window, nullptr);
        glfwSetWindowPosCallback(m_window, nullptr);
        glfwSetWindowFocusCallback(m_window, nullptr);
        glfwSetWindowMaximizeCallback(m_window, nullptr);
        glfwSetWindowIconifyCallback(m_window, nullptr);
        glfwSetWindowContentScaleCallback(m_window, nullptr);
        glfwSetWindowRefreshCallback(m_window, nullptr);

        glfwSetKeyCallback(m_window, nullptr);
        glfwSetCharCallback(m_window, nullptr);

        glfwSetCursorPosCallback(m_window, nullptr);
        glfwSetMouseButtonCallback(m_window, nullptr);
        glfwSetScrollCallback(m_window, nullptr);

        glfwSetWindowUserPointer(m_window, nullptr);
    }

    void EventGenerator::register_event_handler(const AbstractEventHandlerFn<Event>& handler)
    {
        m_handler = handler;
        glfwSetWindowUserPointer(m_window, static_cast<void*>(&m_handler));
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

        glfwSetWindowCloseCallback(m_window,        Callbacks::window_close_callback);
        glfwSetWindowSizeCallback(m_window,         Callbacks::window_size_callback);
        glfwSetFramebufferSizeCallback(m_window,    Callbacks::framebuffer_size_callback);
        glfwSetWindowPosCallback(m_window,          Callbacks::window_pos_callback);
        glfwSetWindowFocusCallback(m_window,        Callbacks::window_focus_callback);
        glfwSetWindowMaximizeCallback(m_window,     Callbacks::window_maximize_callback);
        glfwSetWindowIconifyCallback(m_window,      Callbacks::window_iconify_callback);
        glfwSetWindowContentScaleCallback(m_window, Callbacks::window_content_scale_callback);
        glfwSetWindowRefreshCallback(m_window,      Callbacks::window_refresh_callback);

        glfwSetKeyCallback(m_window,  Callbacks::key_callback);
        glfwSetCharCallback(m_window, Callbacks::char_callback);

        glfwSetCursorPosCallback(m_window,   Callbacks::cursor_pos_callback);
        glfwSetMouseButtonCallback(m_window, Callbacks::mouse_button_callback);
        glfwSetScrollCallback(m_window,      Callbacks::scroll_callback);
    }

    void EventGenerator::handle_event(GLFWwindow* window, Event& event)
    {
        const auto& handler = *static_cast<AbstractEventHandlerFn<Event>*>(glfwGetWindowUserPointer(window));
        {
            DRK_PROFILE_SCOPE(event.get_name());
            handler(event);
        }
        event.log_event();
    }

    ////////// Window Events //////////

    void EventGenerator::Callbacks::window_close_callback(GLFWwindow* window)
    {
        WindowClosedEvent event;
        handle_event(window, event);
    }

    void EventGenerator::Callbacks::window_size_callback(GLFWwindow* window, int width, int height)
    {
        WindowResizedEvent event((uint)width, (uint)height);
        handle_event(window, event);
    }

    void EventGenerator::Callbacks::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        FramebufferResizedEvent event((uint)width, (uint)height);
        handle_event(window, event);
    }

    void EventGenerator::Callbacks::window_pos_callback(GLFWwindow* window, int xPos, int yPos)
    {
        WindowMovedEvent event(xPos, yPos);
        handle_event(window, event);
    }

    void EventGenerator::Callbacks::window_focus_callback(GLFWwindow* window, int focused)
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

    void EventGenerator::Callbacks::window_maximize_callback(GLFWwindow* window, int maximized)
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

    void EventGenerator::Callbacks::window_iconify_callback(GLFWwindow* window, int iconified)
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

    void EventGenerator::Callbacks::window_content_scale_callback(GLFWwindow* window, float xScale, float yScale)
    {
        WindowScaledEvent event(xScale, yScale);
        handle_event(window, event);
    }

    void EventGenerator::Callbacks::window_refresh_callback(GLFWwindow* window)
    {
        WindowRefreshedEvent event;
        handle_event(window, event);
    }

    ////////// Keyboard Events //////////

    void EventGenerator::Callbacks::key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods)
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

    void EventGenerator::Callbacks::char_callback(GLFWwindow* window, uint codePoint)
    {
        CharTypedEvent event(static_cast<char>(codePoint));
        handle_event(window, event);
    }

    ////////// Mouse Events //////////

    void EventGenerator::Callbacks::cursor_pos_callback(GLFWwindow* window, double xPos, double yPos)
    {
        MouseMovedEvent event((float)xPos, (float)yPos);
        handle_event(window, event);
    }

    void EventGenerator::Callbacks::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
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

    void EventGenerator::Callbacks::scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
    {
        ScrollWheelMovedEvent event((float)xOffset, (float)yOffset);
        handle_event(window, event);
    }
}
