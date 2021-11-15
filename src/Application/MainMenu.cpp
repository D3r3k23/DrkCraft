#include "MainMenu.hpp"

#include "Application.hpp"
#include "Game/Game.hpp"
#include "Core/Profiler.hpp"

#include <imgui/imgui.h>

namespace DrkCraft
{
    MainMenu::MainMenu(void)
      : Layer("MainMenuLayer", true),
        m_settingsMenu(Layer::create<SettingsMenu>(false))
    {

    }

    MainMenu::~MainMenu(void)
    {

    }

    void MainMenu::on_attach(void)
    {
        m_settingsMenu->set_close_callback_fn([&]
        {
            activate_layer();
        });
        Application::get_instance().add_overlay(m_settingsMenu);
    }

    void MainMenu::on_detach(void)
    {
        m_settingsMenu->detach_layer();
    }

    void MainMenu::on_update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();
    }

    void MainMenu::on_render(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        static bool showDemoWindow = true;
        if (showDemoWindow)
            ImGui::ShowDemoWindow(&showDemoWindow);


        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize;
        ImGui::Begin("MainMenu", nullptr, windowFlags);


        ImGui::End();
    }

    void MainMenu::on_event(Event& event)
    {
        DRK_PROFILE_FUNCTION();

        EventDispatcher ed(event);
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));
        ed.dispatch<MouseButtonPressedEvent>(DRK_BIND_FN(on_mouse_button_pressed));
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
                DRK_LOG_CORE_INFO("MainMenu: Exit application");
                detach_layer();
                return true;
            }
            default:
                return false;
        }
    }

    bool MainMenu::on_mouse_button_pressed(MouseButtonPressedEvent& event)
    {
        if (event.button == MouseCode::Right)
        {
            m_settingsMenu->activate_layer();
            deactivate_layer();
            return true;
        }
        else
            return false;
    }
}
