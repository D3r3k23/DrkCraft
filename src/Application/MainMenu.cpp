#include "MainMenu.hpp"

#include "Application.hpp"
#include "Game/Game.hpp"
#include "Core/Profiler.hpp"

#include <imgui/imgui.h>

namespace DrkCraft
{
    MainMenu::MainMenu(void)
      : Layer("MainMenuLayer", true)
    {

    }

    MainMenu::~MainMenu(void)
    {

    }

    void MainMenu::on_attach(void)
    {

    }

    void MainMenu::on_detach(void)
    {

    }

    void MainMenu::on_update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();
    }

    void MainMenu::on_render(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        ImGui::ShowDemoWindow();
    }

    void MainMenu::on_event(Event& event)
    {
        DRK_PROFILE_FUNCTION();

        EventDispatcher ed(event);
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));
    }

    bool MainMenu::on_key_pressed(KeyPressedEvent& event)
    {
        switch (event.key)
        {
            case KeyCode::Enter:
            {
                Application::get_instance().add_layer(Layer::create<Game>());
                detach_layer();
                return true;
            }
            case KeyCode::Escape:
            {
                DRK_LOG_INFO("MainMenu: Exit application");
                detach_layer();
                return true;
            }
            default:
                return false;
        }
    }
}
