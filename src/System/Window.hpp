#ifndef DRK_WINDOW_HPP
#define DRK_WINDOW_HPP

#include "Core/Base.hpp"

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

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

        // void show_cursor(bool show);

        glm::ivec2 get_pos(void) const;

        glm::uvec2 resize(uint width, uint height);
        glm::uvec2 resize(glm::uvec2 size);

        glm::uvec2 get_size(void) const;
        glm::uvec2 get_framebuffer_size(void) const;

        glm::vec2 get_content_scale(void) const;

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

#endif // DRK_WINDOW_HPP
