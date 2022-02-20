#include "Monitor.hpp"

#include "Core/Settings.hpp"
#include "Core/Debug/Profiler.hpp"

#include <vector>
#include <algorithm>
#include <execution>
#include <mutex>
namespace DrkCraft
{
    /////////////////////////
    //       Monitor       //
    /////////////////////////

    Monitor::Monitor(GLFWmonitor* monitor, uint number, AbstractEventHandlerFn<MonitorEvent>& eventHandler)
      : m_monitor(monitor),
        m_number(number)
    {
        m_vidMode = find_best_vid_mode();
        glfwSetMonitorUserPointer(m_monitor, static_cast<void*>(&eventHandler));
    }

    Monitor::~Monitor(void)
    {
        if (m_monitor)
            glfwSetMonitorUserPointer(m_monitor, nullptr);
    }

    Monitor::Monitor(Monitor&& other)
    {
        m_monitor = other.m_monitor;
        m_number  = other.m_number;

        other.m_monitor = nullptr;
        other.m_number  = 0;

        m_vidMode = other.m_vidMode;
        m_scale   = other.m_scale;
    }

    Monitor& Monitor::operator=(Monitor&& other)
    {
        if (m_monitor)
            glfwSetMonitorUserPointer(m_monitor, nullptr);

        m_monitor = other.m_monitor;
        m_number  = other.m_number;

        other.m_monitor = nullptr;
        other.m_number  = 0;

        m_vidMode = other.m_vidMode;
        m_scale   = other.m_scale;

        return *this;
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

    ivec2 Monitor::get_resolution(void) const
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

    #if 0
        DRK_LOG_CORE_DEBUG("Monitor: {}", get_name());
        for (int i = 0; i < count; i++)
            DRK_LOG_CORE_DEBUG("vidMode[{}]: W={} H={} R={} G={} B={} RR={}", i,
                vidModesPtr[i].width,
                vidModesPtr[i].height,
                vidModesPtr[i].redBits,
                vidModesPtr[i].greenBits,
                vidModesPtr[i].blueBits,
                vidModesPtr[i].refreshRate);
    #endif

        std::vector<VidMode> vidModes(vidModesPtr, vidModesPtr + count);
        std::partial_sort(vidModes.begin(), vidModes.begin(), vidModes.end(), [](VidMode& v1, VidMode& v2)
        {
            // Sort first by refresh rate, then by total resolution area
            if (v1.refreshRate == v2.refreshRate)
                return v1.width * v1.height > v2.width * v2.height;
            else
                return v1.refreshRate > v2.refreshRate;
        });

        for (int i = 0; i < count; i++)
            DRK_LOG_CORE_DEBUG("vidMode[{}]: W={} H={} R={} G={} B={} RR={}", i,
                vidModes[i].width,
                vidModes[i].height,
                vidModes[i].redBits,
                vidModes[i].greenBits,
                vidModes[i].blueBits,
                vidModes[i].refreshRate);

        return *(vidModes.begin());
    }

    bool Monitor::operator==(const Monitor& other) const
    {
        return m_monitor == other.m_monitor;
    }

    bool Monitor::operator!=(const Monitor& other) const
    {
        return !(*this == other);
    }

    ////////////////////////////////
    //       MonitorManager       //
    ////////////////////////////////

    MonitorManager::MonitorManager(void)
      : m_fullscreen(false)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Creating MonitorManager");

        glfwSetMonitorCallback(monitor_event_callback);
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

    void MonitorManager::load_monitors(void)
    {
        DRK_PROFILE_FUNCTION();

        int count;
        GLFWmonitor** glfwMonitorsPtr;
        {
            DRK_PROFILE_SCOPE("glfwGetMonitors");
            glfwMonitorsPtr = glfwGetMonitors(&count);
        }
        DRK_ASSERT_CORE(count > 0, "glfwGetMonitors error");

        std::vector<std::pair<uint, GLFWmonitor*>> glfwMonitors;
        for (uint i = 0; i < count; i++)
            glfwMonitors.emplace_back(i, glfwMonitorsPtr[i]);

        std::mutex mutex;
        // For some reason the ranges:: version of these doesnt work
        std::for_each(std::execution::par_unseq, glfwMonitors.begin(), glfwMonitors.end(), [this, &mutex](const auto& monitorPair)
        {
            DRK_PROFILE_SCOPE("Monitor load");

            const auto& [number, glfwMonitor] = monitorPair;
            Monitor monitor(glfwMonitor, number, m_eventHandler);

            std::lock_guard lock(mutex);
            m_monitors.push_back(std::move(monitor));
        });
        {
            DRK_PROFILE_SCOPE("Sort monitors");
            std::ranges::sort(m_monitors, [](const Monitor& first, const Monitor& second)
            {
                return first.get_number() < second.get_number();
            });
        }
    }

    void MonitorManager::refresh_monitors(void)
    {
        m_monitors.clear();
        load_monitors();
    }

    uint MonitorManager::get_monitor_number(GLFWmonitor* rawMonitor) const
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
            number = 0;
        }
        DRK_ASSERT_DEBUG_NO_MSG(m_monitors[number].get_number() == number);
        return m_monitors[number];
    }

    int MonitorManager::get_fullscreen_monitor(void) const
    {
        if (m_fullscreenMonitor)
            return *m_fullscreenMonitor;
        else
            return -1;
    }

    void MonitorManager::activate_fullscreen(Window& window, uint monitor)
    {
        if (monitor >= num_monitors())
        {
            DRK_LOG_CORE_ERROR("Monitor {} not found", monitor);
            DRK_ASSERT_DEBUG_NO_MSG(false);
            return;
        }
        if (!fullscreen_activated())
        {
            m_fullscreen = true;
            m_fullscreenMonitor = monitor;
            m_savedWindowedSize = window.get_size();
            m_savedWindowedPos  = window.get_pos();
        }
        const GLFWvidmode& vidMode = get_monitor(monitor).get_vid_mode();
        GLFWmonitor*    rawMonitor = get_monitor(monitor).get_raw_monitor();

        glfwSetWindowMonitor(window.get_raw_window(), rawMonitor, 0, 0, vidMode.width, vidMode.height, vidMode.refreshRate);
    }

    void MonitorManager::deactivate_fullscreen(Window& window)
    {
        const auto& defaultSize = RuntimeSettings::get_config().init_window_size;
        const auto& size = m_savedWindowedSize.value_or(defaultSize);
        const auto& pos  = m_savedWindowedPos.value_or(ivec2(0, 0));

        m_fullscreen = false;
        glfwSetWindowMonitor(window.get_raw_window(), nullptr, pos.x, pos.y, size.x, size.y, 0);
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
