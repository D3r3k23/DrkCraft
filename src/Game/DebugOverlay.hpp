#ifndef DRK_DEBUG_OVERLAY_HPP
#define DRK_DEBUG_OVERLAY_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Timestep.hpp"
#include "Application/Events.hpp"

namespace DrkCraft
{
    class DebugOverlay : public Layer
    {
    public:
        DebugOverlay(bool activate=false);
        ~DebugOverlay(void);

        void on_attach(void) override;
        void on_detach(void) override;

        void on_update(Timestep timestep) override;
        void on_render(Timestep timestep) override;
        void on_event(Event& event) override;

    private:

    private:

    };
}

#endif // DRK_DEBUG_OVERLAY_HPP