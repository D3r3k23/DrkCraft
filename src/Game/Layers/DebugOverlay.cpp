#include "DebugOverlay.hpp"

#include "Application/Application.hpp"
#include "System/Monitor.hpp"
#include "Audio/Audio.hpp"
#include "Core/Debug/Profiler.hpp"

#include <imgui/imgui.h>

namespace DrkCraft
{
    ////////////////////////////
    //       FpsCounter       //
    ////////////////////////////

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

    //////////////////////////////
    //       DebugOverlay       //
    //////////////////////////////

    DebugOverlay::DebugOverlay(const AssetManager& assetManager, ImGuiManager& imguiManager, bool activate)
      : Layer("DebugOverlayLayer", activate),
        m_assetManager(assetManager),
        m_imguiManager(imguiManager),
        m_currentFps(0.995f),
        m_avgFps(0.75f)
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
        m_currentFps.update(timestep);
        if (m_fpsAvgTimer.on_interval<1000>())
            m_avgFps.update(m_currentFps.get_avg_frame_time());
    }

    void DebugOverlay::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize({250, 200});
        ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize);

        // FPS
        ImGui::BeginGroup();
        {
            ImGui::Text("FPS (Current): %.1f", m_currentFps.get_fps());
            ImGui::Text("FPS (Average): %.1f", m_avgFps.get_fps());
        }
        ImGui::EndGroup();

        // Display info
        ImGui::BeginGroup();
        {
            if (Application::get_instance().is_fullscreen())
            {
                auto& monitorManager = Application::get_monitors();
                const auto& monitor = monitorManager.get_monitor(monitorManager.get_fullscreen_monitor());
                const auto& res    = monitor.get_resolution();

                ImGui::Text("Fullscreen (Monitor %u: %s)", monitor.get_number(), monitor.get_name().data());
                ImGui::Text("%dx%d, %u Hz", res.x, res.y, monitor.get_refresh_rate());
            }
            else
            {
                const auto& window = Application::get_window();
                const auto& res = window.get_framebuffer_size();

                ImGui::Text("Window: %dx%d", res.x, res.y);
            }
        }
        ImGui::EndGroup();

        // Audio info
        ImGui::BeginGroup();
        {
            ImGui::Text("Audio device: %s", Audio::get_device_name());
        }
        ImGui::EndGroup();

        // Renderer stats
        ImGui::BeginGroup();
        {
            ImGui::Text("Renderer:");
            ImGui::BeginGroup();
            ImGui::Text("Draw calls: %u", m_rendererStats.drawCalls);
            ImGui::Text("Indices:   %u", m_rendererStats.indices);
            ImGui::Text("Textures: %u", m_rendererStats.textures);
            ImGui::EndGroup();

            ImGui::Text("CubeRenderer:");
            ImGui::BeginGroup();
            ImGui::Text("Cubes: %u", m_cubeRendererStats.cubes);
            ImGui::Text("Cube faces: %u", m_cubeRendererStats.cubeFaces);
            ImGui::EndGroup();

            ImGui::Text("ImGui:");
            ImGui::BeginGroup();
            ImGui::Text("Draw commands: %u", m_imguiRenderStats.drawCmds);
            ImGui::Text("Draw lists:   %u", m_imguiRenderStats.drawLists);
            ImGui::Text("Indices:    %u", m_imguiRenderStats.indices);
            ImGui::Text("Vertices: %u", m_imguiRenderStats.vertices);
            ImGui::EndGroup();
        }
        ImGui::EndGroup();

        // Asset loading status
        if (m_assetManager.loading())
        {
            ImGui::BeginGroup();
            ImGui::Text("Loading asset: %s", m_assetManager.currently_loading());
            ImGui::EndGroup();
        }

        // World loading status

        // Player location

        ImGui::End();
    }

    void DebugOverlay::on_event(Event& event)
    {
        EventDispatcher ed(event);
    }

    void DebugOverlay::update_renderer_stats(void)
    {
        if (m_renderStatsUpdateTimer.on_interval<1000>())
        {
            m_rendererStats = Renderer::get_stats();
            m_cubeRendererStats = CubeRenderer::get_stats();
        }
    }

    void DebugOverlay::upload_imgui_stats(const ImGuiRenderStats& stats)
    {
        if (m_imguiStatsUpdateTimer.on_interval<2000>())
        {
            m_imguiRenderStats = stats;
        }
    }
}