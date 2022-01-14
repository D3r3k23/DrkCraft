#include "Window.hpp"

#include "Graphics/Renderer.hpp"
#include "Core/RunSettings.hpp"
#include "Core/Util.hpp"
#include "Core/Profiler.hpp"

namespace DrkCraft
{
    Window::Window(std::string_view title)
      : m_title(title)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Creating Window");

        init_raw_window();

        set_vsync(RuntimeSettings::get().vsync);
    }

    Window::~Window(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Destroying Window");

        glfwDestroyWindow(m_window);
    }

    void Window::init_raw_window(void)
    {
        DRK_PROFILE_FUNCTION();

        const auto& config = RuntimeSettings::config();
        int width  = config.init_window_size.width;
        int height = config.init_window_size.height;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
        {
            DRK_PROFILE_SCOPE("glfwCreateWindow");
            m_window = glfwCreateWindow(width, height, m_title.c_str(), nullptr, nullptr);
            DRK_ASSERT_CORE(m_window, "Failed to create GLFW window");
        }{
            DRK_PROFILE_SCOPE("glfwMakeContextCurrent");
            glfwMakeContextCurrent(m_window);
        }
        glfwSetWindowAspectRatio(m_window, 16, 9);
        glfwSetWindowSizeLimits(m_window, width, height, GLFW_DONT_CARE, GLFW_DONT_CARE);

        // Ptr<GLFWimage> icon = load_image("assets/icons/DrkCraft.jpg");
        // glfwSetWindowIcon(m_window, icon.get());
    }

    GLFWwindow* Window::get_raw_window(void) const
    {
        return m_window;
    }

    void Window::poll_events(void)
    {
        DRK_PROFILE_FUNCTION();
        glfwPollEvents();
    }

    void Window::swap_buffers(void)
    {
        DRK_PROFILE_FUNCTION();
        glfwSwapBuffers(m_window);
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

    glm::ivec2 Window::get_pos(void) const
    {
        int x, y;
        glfwGetWindowPos(m_window, &x, &y);
        return {x, y};
    }

    glm::uvec2 Window::resize(uint width, uint height)
    {
        return resize({width, height});
    }

    glm::uvec2 Window::resize(glm::uvec2 size)
    {
        glfwSetWindowSize(m_window, size.x, size.y);
        return size;
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

    glm::vec2 Window::get_content_scale(void) const
    {
        glm::vec2 scale;
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
