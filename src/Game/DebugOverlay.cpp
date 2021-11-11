#include "DebugOverlay.hpp"

namespace DrkCraft
{
    DebugOverlay::DebugOverlay(bool activate)
      : Layer("DebugOverlayLayer", activate)
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

    }

    void DebugOverlay::on_render(Timestep timestep)
    {

    }

    void DebugOverlay::on_event(Event& event)
    {
        EventDispatcher ed(event);
    }
}
