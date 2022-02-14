#ifndef DRK_GAME_LAYERS_DEBUG_OVERLAY_HPP
#define DRK_GAME_LAYERS_DEBUG_OVERLAY_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Application/Timestep.hpp"

#include "System/AssetManager.hpp"
#include "Application/ImGuiTools.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/CubeRenderer.hpp"

namespace DrkCraft
{
    class FpsCounter
    {
    public:
        FpsCounter(float smoothing=0.9f);

        float update(float frameTime);
        float get_fps(void) const;
        float get_avg_frame_time(void) const;

    private:
        const float m_SMOOTHING;
        float m_avgFps;
    };

    class DebugOverlay : public Layer
    {
    public:
        DebugOverlay(const AssetManager& assetManager, ImGuiManager& imguiManager, bool activate=false);
        virtual ~DebugOverlay(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_update(Timestep timestep) override;
        virtual void on_render(void) override;
        virtual void on_event(Event& event) override;

        void update_renderer_stats(void);
        void upload_imgui_stats(const ImGuiRenderStats& stats);

    private:
        const AssetManager& m_assetManager;
        ImGuiManager& m_imguiManager;

        FpsCounter m_currentFps;
        FpsCounter m_avgFps;

        RendererStats m_rendererStats;
        CubeRendererStats m_cubeRendererStats;
        ImGuiRenderStats m_imguiRenderStats;

        IntervalTimer m_fpsAvgTimer;
        IntervalTimer m_renderStatsUpdateTimer;
        IntervalTimer m_imguiStatsUpdateTimer;
    };
}

#endif // DRK_GAME_LAYERS_DEBUG_OVERLAY_HPP
