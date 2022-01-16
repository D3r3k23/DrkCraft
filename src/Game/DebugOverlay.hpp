#ifndef DRK_DEBUG_OVERLAY_HPP
#define DRK_DEBUG_OVERLAY_HPP

//// Could remove this entirely from Release builds?

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Core/Timestep.hpp"
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
        DebugOverlay(bool activate=false);
        virtual ~DebugOverlay(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_update(Timestep timestep) override;
        virtual void on_render(void) override;
        virtual void on_event(Event& event) override;

    private:
        FpsCounter m_currentFps;
        FpsCounter m_avgFps;
        IntervalTimer m_fpsAvgTimer;
    };
}

#endif // DRK_DEBUG_OVERLAY_HPP
