#include "Window.hpp"

#include "Input.hpp"
#include "Core/Profiler.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
    Window::Window(std::string_view title, uint width, uint height, bool enableVsync)
      : m_title(title)
    {
        DRK_PROFILE_FUNCTION();

        init_native_window(title, (int)width, (int)height);
        set_vsync(enableVsync);

        m_eventGenerator = make_ptr<EventGenerator>(m_window);

        DRK_LOG_CORE_TRACE("Glad: Loading OpenGL using GLFW loader function");
        {
            DRK_PROFILE_SCOPE("Load OpenGL");
            int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            DRK_ASSERT_CORE(status, "Glad failed to initialize OpenGL context");
        }
    }

    Window::~Window(void)
    {
        DRK_PROFILE_FUNCTION();

        glfwDestroyWindow(m_window);
    }

    void Window::init_native_window(std::string_view title, int width, int height)
    {
        DRK_PROFILE_FUNCTION();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);

        m_window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
        DRK_ASSERT_CORE(m_window, "Failed to create GLFW window");

        glfwMakeContextCurrent(m_window);

        glfwSetWindowAspectRatio(m_window, 16, 9);
        glfwSetWindowSizeLimits(m_window, 720, 405, GLFW_DONT_CARE, GLFW_DONT_CARE);

        // Ptr<GLFWimage> icon = load_image("assets/icons/DrkCraft.jpg");
        // glfwSetWindowIcon(m_window, icon.get());
    }

    GLFWwindow* Window::get_native_window(void) const
    {
        return m_window;
    }

    void Window::register_event_handler(const AbstractEventHandlerFn& handler)
    {
        DRK_PROFILE_FUNCTION();

        m_eventGenerator->register_event_handler(handler);
        m_eventGenerator->register_event_callbacks();
    }

    void Window::on_update(void)
    {
        DRK_PROFILE_FUNCTION();
        {
            DRK_PROFILE_SCOPE("glfwSwapBuffers");
            glfwSwapBuffers(m_window);
        }{
            DRK_PROFILE_SCOPE("glfwPollEvents");
            glfwPollEvents();
        }
    }

    void Window::set_vsync(bool enable)
    {
        DRK_PROFILE_FUNCTION();

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
        glfwSetWindowSize(m_window, width, height);
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

    std::vector<GLFWmonitor*> Window::get_monitors(void) const
    {
        int count;
        GLFWmonitor** monitors = glfwGetMonitors(&count);
        return std::vector<GLFWmonitor*>(monitors, monitors + count);
    }

    GLFWmonitor* Window::get_primary_monitor(void) const
    {
        return glfwGetPrimaryMonitor();
    }

    void Window::set_fullscreen(void)
    {
        // set_monitor(current);
    }

    void Window::set_monitor(GLFWmonitor* monitor)
    {
        // glfwSetWindowMonitor(m_window, monitor);
    }

    bool Window::is_focused(void) const
    {
        return glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
    }

    bool Window::is_hovered(void) const
    {
        return glfwGetWindowAttrib(m_window, GLFW_HOVERED);
    }

    bool Window::is_maximized(void) const
    {
        return glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED);
    }

    bool Window::is_minimized(void) const
    {
        return glfwGetWindowAttrib(m_window, GLFW_ICONIFIED);
    }

    bool Window::is_fullscreen(void) const
    {
        return glfwGetWindowMonitor(m_window) != nullptr;
    }
}
