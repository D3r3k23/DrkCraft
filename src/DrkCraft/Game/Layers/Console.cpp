#include "Console.hpp"

namespace DrkCraft::Game
{
    Console::Console(bool activate)
      : Layer("DebugLayer", activate)
    {

    }

    Console::~Console(void)
    {

    }

    void Console::on_attach(void)
    {

    }

    void Console::on_detach(void)
    {

    }

    void Console::on_update(Timestep timestep)
    {

    }

    void Console::on_render(void)
    {

    }

    void Console::on_event(Event& event)
    {
        EventDispatcher ed(event);
    }
}
