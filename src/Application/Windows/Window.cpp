#include "../Window.hpp"

#include <glad/glad.h>

#if defined(DRK_PLATFORM_WINDOWS)
    namespace DrkCraft
    {
        Window::Window(std::string_view name, uint width, uint height)
          : m_name(name)
        {
            m_window = glfwCreateWindow(width, height, name.data(), NULL, NULL);
            DRK_ASSERT(m_window, "Failed to create GLFW window");

            glfwMakeContextCurrent(m_window);

            DRK_LOG_TRACE("Glad: Loading OpenGL functions using GLFW loader function");
            int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            DRK_ASSERT(status, "Glad failed to initialize OpenGL context");
        }

        GLFWwindow* Window::get_native_window(void) const
        {
            return m_window;
        }

        void Window::on_update(void)
        {
            glfwPollEvents();
            glfwSwapBuffers(m_window);
        }

        bool Window::should_close(void) const
        {
            return glfwWindowShouldClose(m_window);
        }

        Window::Size Window::get_size(void) const
        {
            int width, height;
            glfwGetWindowSize(m_window, &width, &height);
            return { (uint)width, (uint)height };
        }

        Window::Size Window::resize(Size newSize)
        {
            glfwSetWindowSize(m_window, newSize.width, newSize.height);
            return get_size();
        }

        Window::Size Window::get_framebuffer_size(void) const
        {
            int width, height;
            glfwGetFramebufferSize(m_window, &width, &height);
            return { (uint)width, (uint)height };
        }

        Window::~Window(void)
        {
            glfwDestroyWindow(m_window);
        }
    }
#endif
