#include "../Window.hpp"

#if defined(DRK_PLATFORM_WINDOWS)
    namespace DrkCraft
    {
        Window::Window(std::string_view name, uint width, uint height)
          : m_name(name)
        {
            m_windowHandle = glfwCreateWindow(width, height, name.data(), NULL, NULL);
            DRK_ASSERT(m_windowHandle, "Failed to create GLFW window");

            glfwMakeContextCurrent(m_windowHandle);

            int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            DRK_ASSERT(status, "Failed to initialize Glad");
        }

        void Window::on_update(void)
        {

        }

        Window::Size Window::size(void) const
        {
            int width, height;
            glfwGetWindowSize(m_windowHandle, &width, &height);
            return { (uint)width, (uint)height };
        }

        Window::Size Window::resize(Size newSize)
        {
            glfwSetWindowSize(m_windowHandle, newSize.width, newSize.height);
            return size();
        }

        void Window::on_update(void)
        {

        }
    }
#endif
