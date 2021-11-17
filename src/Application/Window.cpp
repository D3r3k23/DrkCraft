#include "Window.hpp"

#include "Input.hpp"
#include "Core/RunSettings.hpp"
#include "Core/Profiler.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
    Window::Window(std::string_view title)
      : m_title(title)
    {
        DRK_PROFILE_FUNCTION();

        init_raw_window();

        const auto& settings = RuntimeSettings::get();

        if (settings.fullscreen)
            set_fullscreen();

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

    void Window::init_raw_window(void)
    {
        DRK_PROFILE_FUNCTION();

        const auto& config = RuntimeSettings::config();
        int width  = config.init_window_width;
        int height = config.init_window_height;

        Monitor::register_event_handler(DRK_BIND_FN(on_monitor_event));

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
            m_fullscreenMonitor->set_monitor(Monitor::get_fullscreen_monitor());
        }
    }

    void Window::set_fullscreen(void)
    {
        if (is_fullscreen())
            DRK_LOG_CORE_WARN("Window is already fullscreen");
        else
        {
            m_fullscreenMonitor = { Monitor::get_fullscreen_monitor(), get_size(), get_pos() };

            const GLFWvidmode& vidMode = m_fullscreenMonitor->get_monitor().get_best_vid_mode();
            GLFWmonitor*       monitor = m_fullscreenMonitor->get_monitor().get_raw_monitor();

            glfwSetWindowMonitor(m_window, monitor, 0, 0, vidMode.width, vidMode.height, vidMode.refreshRate);
        }
    }

    void Window::set_windowed(void)
    {
        if (!is_fullscreen())
            DRK_LOG_CORE_WARN("Window is already windowed");
        else
        {
            const auto position = m_fullscreenMonitor->get_saved_position();
            const auto size     = m_fullscreenMonitor->get_saved_size();

            glfwSetWindowMonitor(m_window, nullptr, position.x, position.y, size.x, size.y, 0);

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
        // return glfwGetWindowMonitor(m_window) != nullptr;
        return (bool)m_fullscreenMonitor;
    }
}
