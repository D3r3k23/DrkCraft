#ifndef DRK_MONITOR_HPP
#define DRK_MONITOR_HPP

#include "Core/Base.hpp"
#include "Window.hpp"
#include "Application/Events.hpp"

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <vector>
#include <string>
#include <string_view>
#include <optional>

// Should restructure this - each monitor should have a list of video mode
// options - default is highest res -> highest refresh rate -> color

namespace DrkCraft
{
    using VidMode = GLFWvidmode;

    class Monitor
    {
    public:
        Monitor(GLFWmonitor* monitor, uint number, AbstractEventHandlerFn<MonitorEvent>& eventHandler);
        ~Monitor(void);

        GLFWmonitor* get_raw_monitor(void) const;
        const VidMode& get_vid_mode(void) const;

        uint get_number(void) const;
        std::string_view get_name(void) const;

        glm::ivec2 get_resolution(void) const;
        uint get_refresh_rate(void) const;

        const VidMode& find_best_vid_mode(void) const;

        bool operator==(const Monitor& other) const;
        bool operator!=(const Monitor& other) const;

    private:
        // This is invalidated when GLFW monitor configuration changes
        GLFWmonitor* m_monitor;
        VidMode m_vidMode;
        glm::vec2 m_scale;
        uint m_number;
    };

    class MonitorManager
    {
    public:
        MonitorManager(void);
        ~MonitorManager(void);

        void register_event_handler(const AbstractEventHandlerFn<MonitorEvent>& eventHandler);
        void refresh_monitors(void);

        uint get_monitor_number(GLFWmonitor* rawMonitor);
        Monitor& get_monitor(uint number);
        Monitor& get_primary_monitor(void);
        uint get_fullscreen_monitor(void);

        void activate_fullscreen(const Window& window, uint monitor=0);
        void deactivate_fullscreen(Window& window);

        bool fullscreen_activated(void) const;
        uint num_monitors(void) const;

        const std::vector<Monitor>& monitors(void) const;

    private:
        void handle_event(GLFWmonitor* monitor, MonitorEvent& event);
        static void monitor_event_callback(GLFWmonitor* monitor, int event);

    private:
        std::vector<Monitor> m_monitors;

        AbstractEventHandlerFn<MonitorEvent> m_eventHandler;

        bool m_fullscreen;
        std::optional<glm::uvec2> m_savedWindowedSize;
        std::optional<glm::ivec2> m_savedWindowedPos;
    };
}

#endif // DRK_MONITOR_HPP
