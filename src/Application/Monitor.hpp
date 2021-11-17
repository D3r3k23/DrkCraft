#ifndef DRK_MONITOR_HPP
#define DRK_MONITOR_HPP

#include "Core/Base.hpp"
#include "Events.hpp"

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <vector>
#include <string>
#include <string_view>

// Should restructure this - each monitor should have a list of video mode
// options - default is highest res -> highest refresh rate -> color

namespace DrkCraft
{
    class Monitor
    {
    public:
        static std::vector<Monitor> get_monitors(void);

        static Monitor get_fullscreen_monitor(void);
        static Monitor get_primary_monitor(void);

        static void register_event_handler(const AbstractEventHandlerFn& eventHandler);

        Monitor(GLFWmonitor* monitor, uint number);
        ~Monitor(void);

        GLFWmonitor* get_raw_monitor(void) const;

        uint get_number(void) const;
        std::string_view get_name(void) const;

        glm::ivec2 get_resolution(void) const;
        uint get_refresh_rate(void) const;

        const GLFWvidmode& get_best_vid_mode(void) const;

    private:
        // This is invalidated when GLFW monitor configuration changes
        GLFWmonitor* m_monitor;
        uint m_number;

        static AbstractEventHandlerFn s_eventHandler;
    };

    class FullscreenMonitor
    {
    public:
        FullscreenMonitor(const Monitor& monitor, const glm::uvec2 currentSize, const glm::ivec2& currentPos);

        void set_monitor(const Monitor& monitor);
        const Monitor& get_monitor(void) const;

        glm::uvec2 get_saved_size(void) const;
        glm::ivec2 get_saved_position(void) const;

    private:
        Monitor m_monitor;
        glm::uvec2 m_savedSize;
        glm::ivec2 m_savedPosition;
    };
}

#endif // DRK_MONITOR_HPP
