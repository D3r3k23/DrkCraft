#include "DebugOverlay.hpp"

#include "Application/Application.hpp"
#include "System/Window.hpp"
#include "Core/Profiler.hpp"

#include <imgui/imgui.h>

namespace DrkCraft
{
    //////// FpsCounter ////////

    FpsCounter::FpsCounter(float smoothing)
      : m_SMOOTHING(smoothing),
        m_avgFps(60.0f)
    { }

    float FpsCounter::update(float frameTime)
    {
        float frameFps = 1.0f / frameTime;
        m_avgFps = (m_avgFps * m_SMOOTHING) + (frameFps * (1.0f - m_SMOOTHING));
        return m_avgFps;
    }

    float FpsCounter::get_fps(void) const
    {
        return m_avgFps;
    }

    float FpsCounter::get_avg_frame_time(void) const
    {
        return 1 / m_avgFps;
    }

    //////// DebugOverlay ////////

    DebugOverlay::DebugOverlay(bool activate)
      : Layer("DebugOverlayLayer", activate),
        m_currentFps(0.995f),
        m_avgFps(0.75f),
        m_fpsAvgTimer(1000)
    { }

    DebugOverlay::~DebugOverlay(void)
    {

    }

    void DebugOverlay::on_attach(void)
    {

    }

    void DebugOverlay::on_detach(void)
    {

    }

    void DebugOverlay::on_update(Timestep timestep)
    {
        m_currentFps.update(timestep);
        if (m_fpsAvgTimer.on_interval())
            m_avgFps.update(m_currentFps.get_avg_frame_time());
    }

    void DebugOverlay::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        ImGui::SetNextWindowPos({0, 0});
        ImGui::Begin("Debug Overlay", nullptr);

        ImGui::BeginGroup();
        ImGui::Text("FPS (Current): %.1f", m_currentFps.get_fps());
        ImGui::Text("FPS (Average): %.1f", m_avgFps.get_fps());
        ImGui::EndGroup();

        if (Application::get_instance().is_fullscreen())
        {
            auto& monitorManager = Application::get_monitors();
            const auto& monitor = monitorManager.get_monitor(monitorManager.get_fullscreen_monitor());
            const auto& res    = monitor.get_resolution();

            ImGui::BeginGroup();
            ImGui::Text("Fullscreen (Monitor %u: %s)", monitor.get_number(), monitor.get_name().data());
            ImGui::Text("%dx%d, %u Hz", res.x, res.y, monitor.get_refresh_rate());
            ImGui::EndGroup();
        }
        else
        {
            const auto& window = Application::get_window();
            const auto& res = window.get_framebuffer_size();

            ImGui::BeginGroup();
            ImGui::Text("Window: %dx%d", res.x, res.y);
            ImGui::EndGroup();
        }

        ImGui::End();
    }

    void DebugOverlay::on_event(Event& event)
    {
        EventDispatcher ed(event);
    }
}
