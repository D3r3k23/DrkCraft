#include "Window.hpp"

#include "Input.hpp"
#include "Core/RunSettings.hpp"
#include "Core/Profiler.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
    Window::Window(std::string_view title)
      : m_title(title),
        m_fullscreen(false)
    {
        DRK_PROFILE_FUNCTION();

        const auto& config   = RuntimeSettings::config();
        const auto& settings = RuntimeSettings::get();

        m_windowedSize     = { config.init_window_width, config.init_window_height };
        m_windowedPosition = { config.init_window_width * 0.5f, config.init_window_height * 0.5f };

        init_raw_window(settings.fullscreen);
        set_vsync(settings.vsync);

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

    void Window::init_raw_window(bool fullscreen)
    {
        DRK_PROFILE_FUNCTION();

        Monitor::register_event_handler(DRK_BIND_FN(on_monitor_event));

        int width, height;
        GLFWmonitor* monitor;

        if (fullscreen)
        {
            m_fullscreen = true;
            m_fullscreenMonitor = Monitor::get_fullscreen_monitor();
            const auto& vidMode = m_fullscreenMonitor->get_best_vid_mode();

            width   = vidMode.width;
            height  = vidMode.height;
            monitor = m_fullscreenMonitor->get_raw_monitor();
        }
        else
        {
            m_fullscreen = false;
            width   = m_windowedSize.x;
            height  = m_windowedSize.y;
            monitor = nullptr;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);

        m_window = glfwCreateWindow(width, height, m_title.c_str(), monitor, nullptr);
        DRK_ASSERT_CORE(m_window, "Failed to create GLFW window");

        glfwMakeContextCurrent(m_window);

        glfwSetWindowAspectRatio(m_window, 16, 9);
        glfwSetWindowSizeLimits(m_window, 720, 405, GLFW_DONT_CARE, GLFW_DONT_CARE);

        // Ptr<GLFWimage> icon = load_image("assets/icons/DrkCraft.jpg");
        // glfwSetWindowIcon(m_window, icon.get());
    }

    GLFWwindow* Window::get_raw_window(void) const
    {
        return m_window;
    }

    void Window::register_event_handler(const AbstractEventHandlerFn& handler)
    {
        DRK_PROFILE_FUNCTION();

        m_eventGenerator->register_window_event_handler(handler);
    }

    void Window::update(void)
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

    void Window::on_monitor_event(Event& event)
    {
        if (event.get_type() == EventType::MonitorDisconnected)
        {
            m_fullscreenMonitor = Monitor::get_fullscreen_monitor();
        }
    }

    void Window::set_fullscreen(void)
    {
        if (!is_fullscreen())
        {
            m_windowedPosition = get_pos();
            m_windowedSize = get_size();

            m_fullscreenMonitor = Monitor::get_fullscreen_monitor();

            const auto& vidMode = m_fullscreenMonitor->get_best_vid_mode();
            auto monitor = m_fullscreenMonitor->get_raw_monitor();

            glfwSetWindowMonitor(m_window, monitor, 0, 0, vidMode.width, vidMode.height, vidMode.refreshRate);
        }
    }

    void Window::set_windowed(void)
    {
        if (is_fullscreen())
        {
            uint width  = m_windowedSize.x;
            uint height = m_windowedSize.y;

            int x = m_windowedPosition.x;
            int y = m_windowedPosition.y;

            glfwSetWindowMonitor(m_window, nullptr, x, y, width, height, 0);
            m_fullscreenMonitor.reset();
        }
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
        m_windowedSize = size;
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
