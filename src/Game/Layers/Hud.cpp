#include "Hud.hpp"

namespace DrkCraft::Game
{
    Hud::Hud(const PlayerController& player, bool activate)
      : Layer("HudLayer", activate),
        m_player(player)
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

    void Hud::on_render(void)
    {

    }

    void Hud::on_event(Event& event)
    {
        EventDispatcher ed(event);
    }
}
