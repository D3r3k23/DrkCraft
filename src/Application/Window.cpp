#include "Window.hpp"

#include "Input.hpp"

#include <glad/glad.h>

// Add fullscreen mode

namespace DrkCraft
{
    Window::Window(std::string_view title, uint width, uint height)
      : m_title(title)
    {
        m_window = glfwCreateWindow(width, height, title.data(), NULL, NULL);
        DRK_ASSERT(m_window, "Failed to create GLFW window");

        glfwMakeContextCurrent(m_window);

        set_vsync(true);

        m_eventGenerator = make_ptr<EventGenerator>(m_window);

        DRK_LOG_TRACE("Glad: Loading OpenGL functions using GLFW loader function");
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        DRK_ASSERT(status, "Glad failed to initialize OpenGL context");
    }

    Window::~Window(void)
    {
        glfwDestroyWindow(m_window);
    }

    void Window::register_event_handler(const AbstractEventHandlerFn& handler)
    {
        m_eventGenerator->register_event_handler(handler);
        m_eventGenerator->register_event_callbacks();
    }

    void Window::on_update(void)
    {
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    void Window::set_vsync(bool enable)
    {
        if (enable)
        {
            glfwSwapInterval(1);
            m_vSync = true;
        }
        else
        {
            glfwSwapInterval(0);
            m_vSync = false;
        }
    }

    bool Window::get_vsync(void) const
    {
        return m_vSync;
    }

    glm::uvec2 Window::resize(uint width, uint height)
    {
        glfwSetWindowSize(m_window, width, height); // Also resize framebuffer?
        return get_size();
    }

    glm::uvec2 Window::resize(glm::uvec2 size)
    {
        glfwSetWindowSize(m_window, size.x, size.y);
        return get_size();
    }

    glm::uvec2 Window::get_size(void) const
    {
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        return { (uint)width, (uint)height };
    }

    glm::uvec2 Window::get_framebuffer_size(void) const
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        return { (uint)width, (uint)height };
    }

    bool Window::is_focused(void) const
    {
        return glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
    }

    bool Window::is_hovered(void) const
    {
        return glfwGetWindowAttrib(m_window, GLFW_HOVERED);
    }

    bool Window::is_minimized(void) const
    {
        return glfwGetWindowAttrib(m_window, GLFW_ICONIFIED);
    }

    bool Window::is_maximized(void) const
    {
        return glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED);
    }

    GLFWwindow* Window::get_native_window(void) const
    {
        return m_window;
    }
}
