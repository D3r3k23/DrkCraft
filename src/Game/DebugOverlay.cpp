#include "DebugOverlay.hpp"

#include <imgui/imgui.h>

namespace DrkCraft
{
    DebugOverlay::DebugOverlay(bool activate)
      : Layer("DebugOverlayLayer", activate),
        m_fpsCurrent(0.0f),
        m_fpsAvg(0.0f)
    {

    }

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
        update_fps(timestep);
    }

    void DebugOverlay::on_render(void)
    {
        ImGui::SetNextWindowPos({0, 0});
        ImGui::Begin("Debug Overlay", nullptr);

        ImGui::Text("FPS (Current): %.1f", m_fpsCurrent);
        ImGui::Text("FPS (Average): %.1f", m_fpsAvg);

        ImGui::End();
    }

    void DebugOverlay::on_event(Event& event)
    {
        EventDispatcher ed(event);
    }

    void DebugOverlay::update_fps(float frameTime)
    {
        static const float SMOOTHING = 0.99;

        m_fpsCurrent = 1.0f / frameTime;

        if (m_fpsAvg <= 0.0f)
            m_fpsAvg = m_fpsCurrent;
        else
            m_fpsAvg = (m_fpsAvg * SMOOTHING) + (m_fpsCurrent * (1.0f - SMOOTHING));
    }
}
