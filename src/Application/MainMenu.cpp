#include "MainMenu.hpp"

namespace DrkCraft
{
    MainMenu::MainMenu(void)
      : Layer("MainMenuLayer")
    {

    }

    void MainMenu::on_update(Timestep timestep)
    {
        game.on_update(timestep);
    }

    void MainMenu::on_render(Timestep timestep)
    {
        game.on_render(timestep);
    }

    void MainMenu::on_event(Event& event)
    {
        game.on_event(event);
    }
}
