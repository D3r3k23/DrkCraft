#include "Hud.hpp"

namespace DrkCraft
{
    Hud::Hud(bool activate)
      : Layer("HudLayer", activate)
    {

    }

    Hud::~Hud(void)
    {

    }

    void Hud::on_attach(void)
    {

    }

    void Hud::on_detach(void)
    {

    }

    void Hud::on_update(Timestep timestep)
    {

    }

    void Hud::on_render(Timestep timestep)
    {

    }

    void Hud::on_event(Event& event)
    {
        EventDispatcher ed(event);
    }
}
