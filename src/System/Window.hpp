#ifndef DRK_SYSTEM_SYSTEM_WINDOW_HPP
#define DRK_SYSTEM_SYSTEM_WINDOW_HPP

#include "Core/Base.hpp"
#include "System/Icon.hpp"

#include "lib/glm/vec2.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <string_view>

namespace DrkCraft
{
    class Window
    {
    public:
        Window(std::string_view title);
        ~Window(void);

        GLFWwindow* get_raw_window(void);

        void set_vsync(bool enable);
        bool get_vsync(void) const;

        void set_icon(const Icon& icon);

        // void show_cursor(bool show);

        ivec2 get_pos(void) const;

        void resize(uint width, uint height);
        void resize(const uvec2& size);

        uvec2 get_size(void) const;
        uvec2 get_framebuffer_size(void) const;

        vec2 get_content_scale(void) const;

        // Redo this API?
        bool is_focused(void) const;
        bool is_hovered(void) const;
        bool is_maximized(void) const;
        bool is_minimized(void) const;

        void maximize(void);
        void minimize(void);
        void restore(void);

    private:
        GLFWwindow* m_window;
        std::string m_title;

        bool m_vsync;
    };
}

#endif // DRK_SYSTEM_SYSTEM_WINDOW_HPP
