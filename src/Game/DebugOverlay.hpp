#ifndef DRK_DEBUG_OVERLAY_HPP
#define DRK_DEBUG_OVERLAY_HPP

//// Could remove this entirely from Release builds?

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Core/Timestep.hpp"
#include "System/AssetManager.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/CubeRenderer.hpp"
#include "Core/Util.hpp"

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
        DebugOverlay(const AssetManager& assetManager, bool activate=false);
        virtual ~DebugOverlay(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_update(Timestep timestep) override;
        virtual void on_render(void) override;
        virtual void on_event(Event& event) override;

        void update_renderer_stats(void);

    private:
        const AssetManager& m_assetManager;

        FpsCounter m_currentFps;
        FpsCounter m_avgFps;

        RendererStats m_rendererStats;
        CubeRendererStats m_cubeRendererStats;

        IntervalTimer m_fpsAvgTimer;
        IntervalTimer m_renderStatsUpdateTimer;
    };
}

#endif // DRK_DEBUG_OVERLAY_HPP
