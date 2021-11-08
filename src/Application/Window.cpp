#include "Window.hpp"

#include "Input.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
    Window::Window(std::string_view title, uint width, uint height)
      : m_title(title)
    {
        m_window = glfwCreateWindow(width, height, title.data(), NULL, NULL);
        DRK_ASSERT(m_window, "Failed to create GLFW window");

        glfwMakeContextCurrent(m_window);

        DRK_LOG_TRACE("Glad: Loading OpenGL functions using GLFW loader function");
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        DRK_ASSERT(status, "Glad failed to initialize OpenGL context");
    }

    Window::~Window(void)
    {
        glfwDestroyWindow(m_window);
    }

    void Window::on_update(void)
    {
        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }

    void Window::register_event_handler(const EventHandlerFn& handler)
    {
        eventGenerator.register_event_handler(m_window, handler);
        eventGenerator.register_event_callbacks();
    }

    GLFWwindow* Window::get_native_window(void) const
    {
        return m_window;
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
}
