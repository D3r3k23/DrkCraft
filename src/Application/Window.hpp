#ifndef DRK_WINDOW_HPP
#define DRK_WINDOW_HPP

#include "Core/Base.hpp"
#include "EventGenerator.hpp"

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <string>
#include <vector>

namespace DrkCraft
{
    class Window
    {
    public:
        Window(std::string_view title, uint width, uint height, bool enableVsync=true);
        ~Window(void);

        void init_native_window(std::string_view title, int width, int height);
        GLFWwindow* get_native_window(void) const;

        void register_event_handler(const AbstractEventHandlerFn& handler);

        void on_update(void);

        void set_vsync(bool enable);
        bool get_vsync(void) const;

        // void show_cursor(bool show);

        glm::uvec2 resize(uint width, uint height);
        glm::uvec2 resize(glm::uvec2 size);

        glm::uvec2 get_size(void) const;
        glm::uvec2 get_framebuffer_size(void) const;

        // Maybe these should be in Application (or Monitor class?)
        // Windowed Borderless: set window W/H <- VideoMode->width,height
        std::vector<GLFWmonitor*> get_monitors(void) const;
        GLFWmonitor* get_primary_monitor(void) const;

        void set_fullscreen(void);
        void set_monitor(GLFWmonitor* monitor);

        bool is_focused(void) const;
        bool is_hovered(void) const;
        bool is_maximized(void) const;
        bool is_minimized(void) const;
        bool is_fullscreen(void) const;

    private:
        GLFWwindow* m_window;
        std::string m_title;

        bool m_vSync; // Limits frame rate to monitor's refresh rate (?)

        Ptr<EventGenerator> m_eventGenerator;
    };
}

#endif // DRK_WINDOW_HPP
