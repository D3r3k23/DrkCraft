#include "DebugLayer.hpp"

namespace DrkCraft
{
    DebugLayer::DebugLayer(void)
      : Layer("DebugLayer", false)
    {

    }

    DebugLayer::~DebugLayer(void)
    {

    }

    void DebugLayer::on_attach(void)
    {

    }

    void DebugLayer::on_detach(void)
    {

    }

    void DebugLayer::on_update(Timestep timestep)
    {

    }

    void DebugLayer::on_render(Timestep timestep)
    {

    }

    void DebugLayer::on_event(Event& event)
    {
        EventDispatcher ed(event);
    }
}
