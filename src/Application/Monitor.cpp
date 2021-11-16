#include "Monitor.hpp"

#include "Core/RunSettings.hpp"

#include <algorithm>

namespace DrkCraft
{
    AbstractEventHandlerFn Monitor::s_eventHandler;

    std::vector<Monitor> Monitor::get_monitors(void)
    {
        std::vector<Monitor> monitors;
        int count;
        GLFWmonitor** glfwMonitors = glfwGetMonitors(&count);
        DRK_ASSERT_CORE(count > 0, "glfwGetMonitors error");

        for (uint i = 0; i < count; i++)
        {
            auto monitor = glfwMonitors[i];
            monitors.emplace_back(monitor, i);
        }
        return monitors;
    }

    Monitor Monitor::get_fullscreen_monitor(void)
    {
        int monitorNum = RuntimeSettings::get().fullscreen_monitor;
        auto monitors = Monitor::get_monitors();

        if (monitorNum >= monitors.size())
        {
            DRK_LOG_CORE_WARN("Invalid monitor selected. Defaulting to primary");
            monitorNum = 0;
        }
        return monitors[monitorNum];
    }

    Monitor Monitor::get_primary_monitor(void)
    {
        return { glfwGetPrimaryMonitor(), 0 };
    }

    void Monitor::register_event_handler(const AbstractEventHandlerFn& eventHandler)
    {
        s_eventHandler = eventHandler;

        glfwSetMonitorCallback([](GLFWmonitor* monitor, int event)
        {
            const auto& handler = *static_cast<AbstractEventHandlerFn*>(glfwGetMonitorUserPointer(monitor));
            switch (event)
            {
                case GLFW_CONNECTED:
                {
                    MonitorConnectedEvent e;
                    handler(e);
                    break;
                }
                case GLFW_DISCONNECTED:
                {
                    MonitorDisconnectedEvent e;
                    handler(e);
                    break;
                }
                default:
                    DRK_ASSERT_DEBUG(false, "Unknown Monitor event");
            }
        });
    }

    Monitor::Monitor(GLFWmonitor* monitor, uint number)
      : m_monitor(monitor),
        m_number(number)
    {
        glfwSetMonitorUserPointer(m_monitor, static_cast<void*>(&s_eventHandler));
    }

    GLFWmonitor* Monitor::get_raw_monitor(void) const
    {
        return m_monitor;
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
        auto vidMode = glfwGetVideoMode(m_monitor);
        return { vidMode->width, vidMode->height };
    }

    uint Monitor::get_refresh_rate(void) const
    {
        return glfwGetVideoMode(m_monitor)->refreshRate;
    }

    const GLFWvidmode& Monitor::get_best_vid_mode(void) const
    {
        int count;
        const GLFWvidmode* vidModesPtr = glfwGetVideoModes(m_monitor, &count);
        DRK_ASSERT_CORE(count, "glfwGetVideoModes error");

        DRK_LOG_CORE_DEBUG("Monitor: {}", glfwGetMonitorName(m_monitor));

        // for (int i = 0; i < count; i++)
        //     DRK_LOG_CORE_DEBUG("vidMode[{}]: W={} H={} R={} G={} B={} RR={}", i,
        //         vidModesPtr[i].width,
        //         vidModesPtr[i].height,
        //         vidModesPtr[i].redBits,
        //         vidModesPtr[i].greenBits,
        //         vidModesPtr[i].blueBits,
        //         vidModesPtr[i].refreshRate);

        std::vector<GLFWvidmode> vidModes(vidModesPtr, vidModesPtr + count);
        std::sort(vidModes.begin(), vidModes.end(), [](GLFWvidmode& v1, GLFWvidmode& v2)
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

        return vidModesPtr[count - 1];
    }
}
