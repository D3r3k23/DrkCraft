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

        void on_update(void);

        Size size(void) const;
        Size resize(Size size);
        ~Window(void);


    private:
        GLFWwindow* m_windowHandle;

        std::string m_name;
    };
}

#endif // WINDOW_HPP
