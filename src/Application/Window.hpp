#ifndef DRK_WINDOW_HPP
#define DRK_WINDOW_HPP

#include "Core/Base.hpp"
#include "Monitor.hpp"
#include "Events.hpp"
#include "EventGenerator.hpp"

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <string>
#include <optional>

namespace DrkCraft
{
    class Window
    {
    public:
        Window(std::string_view title);
        ~Window(void);

        void init_raw_window(void);
        GLFWwindow* get_raw_window(void) const;

        void register_event_handler(const AbstractEventHandlerFn& handler);

        void poll_events(void);
        void swap_buffers(void);

        void on_monitor_event(Event& event);

        void set_fullscreen(void);
        void set_windowed(void);

        void set_vsync(bool enable);
        bool get_vsync(void) const;

        // void show_cursor(bool show);

        glm::ivec2 get_pos(void) const;

        glm::uvec2 resize(uint width, uint height);
        glm::uvec2 resize(glm::uvec2 size);

        glm::uvec2 get_size(void) const;
        glm::uvec2 get_framebuffer_size(void) const;

        void maximize(void);
        void minimize(void);
        void restore(void);

        // Redo this API?
        bool is_focused(void) const;
        bool is_hovered(void) const;
        bool is_maximized(void) const;
        bool is_minimized(void) const;
        bool is_fullscreen(void) const;

    private:
        GLFWwindow* m_window;
        std::string m_title;

        std::optional<FullscreenMonitor> m_fullscreenMonitor;

        bool m_vsync;

        Ptr<EventGenerator> m_eventGenerator;
    };
}

#endif // DRK_WINDOW_HPP
