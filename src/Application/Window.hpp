#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Base.hpp"

#include <string>

namespace DrkCraft
{
    class Window
    {
    public:
        Window(std::string_view name, uint width, uint height);
        virtual ~Window() = default;

        virtual void on_update(void) = 0;

        uint width(void)  const { return m_width;  }
        uint height(void) const { return m_height; }

    private:
        std::string m_name;
        uint m_width;
        uint m_height;
    };
}

#endif // WINDOW_HPP
