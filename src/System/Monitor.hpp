#ifndef DRK_SYSTEM_MONITOR_HPP
#define DRK_SYSTEM_MONITOR_HPP

#include "Core/Base.hpp"
#include "System/Window.hpp"
#include "Application/Events.hpp"

#include "lib/glm/vec2.hpp"

#include <GLFW/glfw3.h> // Move to .cpp and use forward declarations

#include <vector>
#include <string>
#include <string_view>

// Should restructure this - each monitor should have a list of video mode
// options - default is highest res -> highest refresh rate -> color

namespace DrkCraft
{
    using VidMode = GLFWvidmode; // Create own VideoMode class

    class Monitor
    {
        friend class MonitorManager;
    private:
        Monitor(GLFWmonitor* monitor, uint number, AbstractEventHandlerFn<MonitorEvent>* eventHandler);

    public:
        ~Monitor(void);

        Monitor(const Monitor&) = delete;
        Monitor& operator=(const Monitor&) = delete;
        Monitor(Monitor&& other);
        Monitor& operator=(Monitor&& other);

        GLFWmonitor* get_raw_monitor(void) const;
        const VidMode& get_vid_mode(void) const;

        uint get_number(void) const;
        std::string_view get_name(void) const;

        ivec2 get_resolution(void) const;
        uint get_refresh_rate(void) const;

        const VidMode& find_best_vid_mode(void) const;

        bool operator==(const Monitor& other) const;
        bool operator!=(const Monitor& other) const;

    private:
        // This is invalidated when GLFW monitor configuration changes
        GLFWmonitor* m_monitor;
        uint m_number;
        VidMode m_vidMode;
        vec2 m_scale;
    };

    class MonitorManager
    {
    public:
        MonitorManager(void);
        ~MonitorManager(void);

        void register_event_handler(AbstractEventHandlerFn<MonitorEvent> handler);
        void load_monitors(void);
        void refresh_monitors(void);

        uint get_monitor_number(GLFWmonitor* rawMonitor) const;
        bool check_monitor_number(uint number) const;

        Monitor& get_monitor(uint number);
        const Monitor& get_monitor(uint number) const;

        int get_fullscreen_monitor(void) const;

        void activate_fullscreen(Window& window, uint monitor=0);
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
        Optional<uint> m_fullscreenMonitor;
        Optional<uvec2> m_savedWindowedSize;
        Optional<ivec2> m_savedWindowedPos;
    };
}

#endif // DRK_SYSTEM_MONITOR_HPP
