#ifndef DRK_DEBUG_OVERLAY_HPP
#define DRK_DEBUG_OVERLAY_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Timestep.hpp"
#include "Application/Events.hpp"

namespace DrkCraft
{
    class FpsCounter
    {

    };

    class DebugOverlay : public Layer
    {
    public:
        DebugOverlay(bool activate=false);
        ~DebugOverlay(void);

        void on_attach(void) override;
        void on_detach(void) override;

        void on_update(Timestep timestep) override;
        void on_render(void) override;
        void on_event(Event& event) override;

    private:
        void update_fps(float frameTime);

    private:
        float m_fpsCurrent;
        float m_fpsAvg;
    };
}

#endif // DRK_DEBUG_OVERLAY_HPP
