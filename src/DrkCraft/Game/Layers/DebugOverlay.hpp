#ifndef DRK_GAME_LAYERS_DEBUG_OVERLAY_HPP
#define DRK_GAME_LAYERS_DEBUG_OVERLAY_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Util/Timestep.hpp"

#include "Game/Game.hpp"
#include "System/AssetLibrary.hpp"
#include "Application/ImGuiController.hpp"
#include "Util/PeriodicUpdate.hpp"

#include "Graphics/OpenGlContext.hpp"
#include "Graphics/Renderer/Renderer.hpp"
#include "Graphics/Renderer/BlockRenderer.hpp"
#include "Graphics/Renderer/MeshRenderer.hpp"

namespace DrkCraft::Game
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
        DebugOverlay(const Game& game, bool activate=false);
        virtual ~DebugOverlay(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_render(void) override;
        virtual void on_update(Timestep timestep) override;
        virtual void on_event(Event& event) override;

    private:
        void update_renderer_stats(void);

    private:
        const Game& m_game;
        const AssetLibrary& m_assetLibrary;
        const ImGuiController& m_imGuiController;
        const OpenGlContext& m_glContext;

        FpsCounter m_currentFps;
        FpsCounter m_avgFps;

        RendererStats    m_rendererStats;
        BlockRendererStats m_blockRendererStats;
        MeshRendererStats m_meshRendererStats;
        ImGuiRendererStats m_imGuiRendererStats;

        IntervalTimer m_fpsAvgTimer;
        IntervalTimer m_renderStatsUpdateTimer;
    };
}

#endif // DRK_GAME_LAYERS_DEBUG_OVERLAY_HPP
