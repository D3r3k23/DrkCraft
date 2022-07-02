#include "DebugOverlay.hpp"

#include "Application/Application.hpp"
#include "Util/ImGui.hpp"
#include "System/Monitor.hpp"
#include "Audio/Audio.hpp"
#include "Core/Debug/Profiler.hpp"

#include <imgui/imgui.h>

namespace DrkCraft::Game
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

    DebugOverlay::DebugOverlay(const Game& game, bool activate)
      : Layer("DebugOverlayLayer", activate),
        m_game(game),
        m_library(Application::get_library()),
        m_imGuiController(Application::get_imgui()),
        m_glContext(Application::get_gl_context()),
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
                auto& monitorManager = Application::get_monitor_manager();
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

        // Hardware renderer info
        ImGui::BeginGroup();
        {
            // Probably should just store this in a member variable
        }

        // Audio info
        ImGui::BeginGroup();
        {
            ImGui::Text("Audio device: %s", Audio::get_device_name());
            // Store this as well?
        }
        ImGui::EndGroup();

        // Renderer stats
        ImGui::BeginGroup();
        {
            ImGui::Text("Renderer:");
            ImGui::BeginGroup();
            ImGui::Text("Draw calls: %u", m_rendererStats.drawCalls);
            ImGui::Text("Indices: %u",   m_rendererStats.indices);
            ImGui::Text("Lines: %u",    m_rendererStats.lines);
            ImGui::Text("Triangles: %u", m_rendererStats.triangles);
            ImGui::EndGroup();

            ImGui::Text("BlockRenderer:");
            ImGui::BeginGroup();
            ImGui::Text("Blocks: %u", m_blockRendererStats.blocks);
            ImGui::Text("Block faces: %u", m_blockRendererStats.blockFaces);
            ImGui::EndGroup();

            ImGui::Text("MeshRenderer:");
            ImGui::BeginGroup();
            ImGui::Text("Meshes: %u", m_meshRendererStats.meshes);
            ImGui::EndGroup();

            ImGui::Text("ImGui:");
            ImGui::BeginGroup();
            ImGui::Text("Draw commands: %u", m_imGuiRendererStats.drawCmds);
            ImGui::Text("Draw lists:   %u", m_imGuiRendererStats.drawLists);
            ImGui::Text("Indices:    %u", m_imGuiRendererStats.indices);
            ImGui::Text("Vertices: %u", m_imGuiRendererStats.vertices);
            ImGui::EndGroup();
        }
        ImGui::EndGroup();

        // Asset loading status
        if (const auto asset = m_library.currently_loading(); asset)
        {
            ImGui::BeginGroup();
            ImGui::Text("Loading asset: %s", *asset);
            ImGui::EndGroup();
        }

        // World loading status

        // Time
        ImGui::BeginGroup();
        {
            const auto& time = m_game.get_time();
            ImGui::Text("D: {} H: {}", time.day(), time.time());
        }
        ImGui::EndGroup();

        // Player info
        ImGui::BeginGroup();
        {

        }
        ImGui::EndGroup();

        // Block info
        ImGui::BeginGroup();
        {

        }
        ImGui::EndGroup();

        ImGui::End();
    }

    void DebugOverlay::on_update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        m_currentFps.update(timestep);
        if (m_fpsAvgTimer.on_interval<1000>())
            m_avgFps.update(m_currentFps.get_avg_frame_time());

        if (m_renderStatsUpdateTimer.on_interval<1000>())
            update_renderer_stats();
    }

    void DebugOverlay::on_event(Event& event)
    {
        EventDispatcher ed(event);
    }

    void DebugOverlay::update_renderer_stats(void)
    {
        m_rendererStats    = Renderer::get_stats();
        m_blockRendererStats = BlockRenderer::get_stats();
        m_meshRendererStats   = MeshRenderer::get_stats();
        m_imGuiRendererStats   = m_imGuiController.get_renderer_stats();
    }
}
