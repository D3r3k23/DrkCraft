#ifndef DRK_GAME_LAYERS_DEBUG_OVERLAY_HPP
#define DRK_GAME_LAYERS_DEBUG_OVERLAY_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Application/Timestep.hpp"

#include "Game/Game.hpp"
#include "System/AssetManager.hpp"
#include "Util/ImGui.hpp"
#include "Graphics/OpenGlContext.hpp"
#include "Graphics/Renderer/Renderer.hpp"
#include "Graphics/Renderer/CubeRenderer.hpp"
#include "Util/PeriodicUpdate.hpp"

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
        DebugOverlay(bool activate=false);

        virtual ~DebugOverlay(void);

        void attach_game(Ref<Game> game);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_render(void) override;
        virtual void on_update(Timestep timestep) override;
        virtual void on_event(Event& event) override;

    private:
        void update_renderer_stats(void);

    private:
        const AssetManager& m_assetManager;
        const ImGuiManager& m_imGuiManager;
        const OpenGlContext& m_glContext;
        Ref<Game> m_game;

        FpsCounter m_currentFps;
        FpsCounter m_avgFps;

        RendererStats      m_rendererStats;
        CubeRendererStats m_cubeRendererStats;
        ImGuiRendererStats m_imGuiRendererStats;

        IntervalTimer m_fpsAvgTimer;
        IntervalTimer m_renderStatsUpdateTimer;
    };
}

#endif // DRK_GAME_LAYERS_DEBUG_OVERLAY_HPP
