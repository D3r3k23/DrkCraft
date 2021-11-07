#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Core/Base.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

namespace DrkCraft
{
    class Window
    {
    public:
        struct Size
        {
            uint width;
            uint height;
        };

        Window(std::string_view name, uint width, uint height);

        GLFWwindow* get_window(void) const;

        void on_update(void);
        bool should_close(void) const;

        Size get_size(void) const;
        Size resize(Size size);

        Size get_framebuffer_size(void) const;

        ~Window(void);

    private:
        GLFWwindow* m_window;

        std::string m_name;
    };
}

#endif // WINDOW_HPP
