#include "Window.hpp"

#include "Core/Settings.hpp"
#include "Core/Debug/Profiler.hpp"

#include <GLFW/glfw3.h>

namespace DrkCraft
{
    Window::Window(std::string_view title)
      : m_title(title)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Creating Window");

        const auto& config = RuntimeSettings::get_config();
        int width  = config.init_window_size.width;
        int height = config.init_window_size.height;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
        {
            DRK_PROFILE_SCOPE("glfwCreateWindow");
            m_window = glfwCreateWindow(width, height, m_title.c_str(), nullptr, nullptr);
            DRK_ASSERT_CORE(m_window, "Failed to create GLFW window");
        }
        glfwSetWindowAspectRatio(m_window, 16, 9);
        glfwSetWindowSizeLimits(m_window, width, height, GLFW_DONT_CARE, GLFW_DONT_CARE);
    }

    Window::~Window(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Destroying Window");

        glfwDestroyWindow(m_window);
    }

    GLFWwindow* Window::get_raw_window(void)
    {
        return m_window;
    }

    const GLFWwindow* Window::get_raw_window(void) const
    {
        return m_window;
    }

    void Window::set_vsync(bool enable)
    {
        DRK_PROFILE_FUNCTION();

        if (enable)
        {
            glfwSwapInterval(1);
            m_vsync = true;
        }
        else
        {
            glfwSwapInterval(0);
            m_vsync = false;
        }
    }

    bool Window::get_vsync(void) const
    {
        return m_vsync;
    }

    void Window::set_icon(const Icon& icon)
    {
        DRK_PROFILE_FUNCTION();

        GLFWimage image = {
            .width  = static_cast<int>(icon.get_size().x),
            .height = static_cast<int>(icon.get_size().y),
            .pixels = const_cast<unsigned char*>(icon.get_image_data())
        };
        glfwSetWindowIcon(m_window, 1, &image);
    }

    ivec2 Window::get_pos(void) const
    {
        int x, y;
        glfwGetWindowPos(m_window, &x, &y);
        return {x, y};
    }

    void Window::resize(uint width, uint height)
    {
        glfwSetWindowSize(m_window, width, height);
    }

    void Window::resize(const uvec2& size)
    {
        resize(size.x, size.y);
    }

    uvec2 Window::get_size(void) const
    {
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        return { (uint)width, (uint)height };
    }

    uvec2 Window::get_framebuffer_size(void) const
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        return { (uint)width, (uint)height };
    }

    vec2 Window::get_content_scale(void) const
    {
        vec2 scale;
        glfwGetWindowContentScale(m_window, &scale.x, &scale.y);
        return scale;
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

    void Window::maximize(void)
    {
        glfwMaximizeWindow(m_window);
    }

    void Window::minimize(void)
    {
        glfwIconifyWindow(m_window);
    }

    void Window::restore(void)
    {
        glfwRestoreWindow(m_window);
    }
}
