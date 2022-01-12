#include "Monitor.hpp"

#include "Core/RunSettings.hpp"
#include "Core/Profiler.hpp"

#include <algorithm>

namespace DrkCraft
{
    Monitor::Monitor(GLFWmonitor* monitor, uint number, AbstractEventHandlerFn<MonitorEvent>& eventHandler)
      : m_monitor(monitor),
        m_number(number)
    {
        m_vidMode = find_best_vid_mode();
        glfwSetMonitorUserPointer(m_monitor, static_cast<void*>(&eventHandler));
    }

    Monitor::~Monitor(void)
    {
        glfwSetMonitorUserPointer(m_monitor, nullptr);
    }

    GLFWmonitor* Monitor::get_raw_monitor(void) const
    {
        return m_monitor;
    }

    const VidMode& Monitor::get_vid_mode(void) const
    {
        return m_vidMode;
    }

    uint Monitor::get_number(void) const
    {
        return m_number;
    }

    std::string_view Monitor::get_name(void) const
    {
        return glfwGetMonitorName(m_monitor);
    }

    glm::ivec2 Monitor::get_resolution(void) const
    {
        return { m_vidMode.width, m_vidMode.height };
    }

    uint Monitor::get_refresh_rate(void) const
    {
        return m_vidMode.refreshRate;
    }

    const VidMode& Monitor::find_best_vid_mode(void) const
    {
        DRK_PROFILE_FUNCTION();

        int count;
        const GLFWvidmode* vidModesPtr;
        {
            DRK_PROFILE_SCOPE("glfwGetVideoModes");
            vidModesPtr = glfwGetVideoModes(m_monitor, &count);
        }
        DRK_ASSERT_CORE(count, "glfwGetVideoModes error");

        DRK_LOG_CORE_DEBUG("Monitor: {}", get_name());

        for (int i = 0; i < count; i++)
            DRK_LOG_CORE_DEBUG("vidMode[{}]: W={} H={} R={} G={} B={} RR={}", i,
                vidModesPtr[i].width,
                vidModesPtr[i].height,
                vidModesPtr[i].redBits,
                vidModesPtr[i].greenBits,
                vidModesPtr[i].blueBits,
                vidModesPtr[i].refreshRate);

        return vidModesPtr[count - 1];

    #if 0
        std::vector<VidMode> vidModes(vidModesPtr, vidModesPtr + count);
        std::ranges::sort(vidModes, [](VidMode& v1, VidMode& v2)
        {
            return v1.width * v1.height > v2.width * v2.height;
        });

        for (int i = 0; i < count; i++)
            DRK_LOG_CORE_DEBUG("vidMode[{}]: W={} H={} R={} G={} B={} RR={}", i,
                vidModes[i].width,
                vidModes[i].height,
                vidModes[i].redBits,
                vidModes[i].greenBits,
                vidModes[i].blueBits,
                vidModes[i].refreshRate);

        return *vidModes.begin();
    #endif
    }

    bool Monitor::operator==(const Monitor& other) const
    {
        return m_monitor == other.m_monitor;
    }

    bool Monitor::operator!=(const Monitor& other) const
    {
        return !(*this == other);
    }

    MonitorManager::MonitorManager(void)
      : m_fullscreen(false)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Creating MonitorManager");

        glfwSetMonitorCallback(monitor_event_callback);
        refresh_monitors();
    }

    MonitorManager::~MonitorManager(void)
    {
        DRK_LOG_CORE_TRACE("Destroying MonitorManager");
        glfwSetMonitorCallback(nullptr);
    }

    void MonitorManager::register_event_handler(const AbstractEventHandlerFn<MonitorEvent>& eventHandler)
    {
        m_eventHandler = eventHandler;
    }

    void MonitorManager::refresh_monitors(void)
    {
        DRK_PROFILE_FUNCTION();

        m_monitors.clear();

        int count;
        GLFWmonitor** glfwMonitors;
        {
            DRK_PROFILE_SCOPE("glfwGetMonitors");
            glfwMonitors = glfwGetMonitors(&count);
        }
        DRK_ASSERT_CORE(count > 0, "glfwGetMonitors error");

        for (uint i = 0; i < count; i++) // This is slow
        {
            auto monitor = glfwMonitors[i];
            m_monitors.emplace_back(monitor, i, m_eventHandler);
        }
    }

    uint MonitorManager::get_monitor_number(GLFWmonitor* rawMonitor)
    {
        for (const auto& monitor : m_monitors)
            if (rawMonitor == monitor.get_raw_monitor())
                return monitor.get_number();

        DRK_ASSERT_DEBUG(false, "Monitor not found");
        return 0;
    }

    Monitor& MonitorManager::get_monitor(uint number)
    {
        if (number >= num_monitors())
        {
            DRK_LOG_CORE_WARN("Invalid monitor selected. Defaulting to primary");
            return get_primary_monitor();
        }
        return m_monitors[number];
    }

    Monitor& MonitorManager::get_primary_monitor(void)
    {
        return m_monitors[0];
    }

    uint MonitorManager::get_fullscreen_monitor(void)
    {
        int num = RuntimeSettings::get().fullscreen_monitor;
        if (num < 0 || num >= num_monitors())
        {
            num = 0;
            RuntimeSettings::get().fullscreen_monitor = num;
        }
        return num;
    }

    void MonitorManager::activate_fullscreen(const Window& window, uint monitor)
    {
        if (monitor >= num_monitors())
        {
            DRK_LOG_CORE_ERROR("Monitor {} not found", monitor);
            DRK_ASSERT_DEBUG_NO_MSG(false);
            return;
        }
        if (!fullscreen_activated())
        {
            m_savedWindowedSize = window.get_size();
            m_savedWindowedPos  = window.get_pos();
            m_fullscreen = true;
        }
        const GLFWvidmode& vidMode = get_monitor(monitor).get_vid_mode();
        GLFWmonitor*    rawMonitor = get_monitor(monitor).get_raw_monitor();

        glfwSetWindowMonitor(window.get_raw_window(), rawMonitor, 0, 0, vidMode.width, vidMode.height, vidMode.refreshRate);
    }

    void MonitorManager::deactivate_fullscreen(Window& window)
    {
        m_fullscreen = false;
        const auto& size = *m_savedWindowedSize;
        const auto& pos  = *m_savedWindowedPos;

        glfwSetWindowMonitor(window.get_raw_window(), nullptr, pos.x, pos.y, size.x, size.y, 0);

        m_savedWindowedSize.reset();
        m_savedWindowedPos.reset();
    }

    bool MonitorManager::fullscreen_activated(void) const
    {
        return m_fullscreen;
    }

    uint MonitorManager::num_monitors(void) const
    {
        return m_monitors.size();
    }

    const std::vector<Monitor>& MonitorManager::monitors(void) const
    {
        return m_monitors;
    }

    void MonitorManager::handle_event(GLFWmonitor* monitor, MonitorEvent& event)
    {
        {
            DRK_PROFILE_SCOPE(event.get_name());
            m_eventHandler(event);
        }
        event.log_event();
    }

    void MonitorManager::monitor_event_callback(GLFWmonitor* monitor, int event)
    {
        MonitorManager& manager = *static_cast<MonitorManager*>(glfwGetMonitorUserPointer(monitor));
        uint number = manager.get_monitor_number(monitor);
        switch (event)
        {
            case GLFW_CONNECTED:
            {
                MonitorConnectedEvent monitorEvent(number);
                manager.handle_event(monitor, monitorEvent);
                break;
            }
            case GLFW_DISCONNECTED:
            {
                MonitorDisconnectedEvent monitorEvent(number);
                manager.handle_event(monitor, monitorEvent);
                break;
            }
            default:
                DRK_ASSERT_DEBUG(false, "Unknown Monitor event");
        }
    }
}
