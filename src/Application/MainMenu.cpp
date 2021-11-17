#include "MainMenu.hpp"

#include "Application.hpp"
#include "ImGuiTools.hpp"
#include "Game/Game.hpp"
#include "Core/Profiler.hpp"

#include <imgui/imgui.h>

namespace DrkCraft
{
    MainMenu::MainMenu(void)
      : Layer("MainMenuLayer", true),
        m_settingsMenu(Layer::create<SettingsMenu>(false)),
        m_show(true)
    {
        m_settingsMenu->set_close_callback_fn(DRK_BIND_FN(show));
    }

    void MainMenu::show(void)
    {
        m_show = true;
    }

    void MainMenu::hide(void)
    {
        m_show = false;
    }

    void MainMenu::on_attach(void)
    {
        Application::get_instance().add_overlay(m_settingsMenu);
    }

    void MainMenu::on_detach(void)
    {
        m_settingsMenu->detach_layer();
    }

    void MainMenu::on_update(Timestep timestep)
    {
        // Animate background?
    }

    void MainMenu::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        // Show background

        if (m_show)
        {
            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration    | ImGuiWindowFlags_NoMove
                                         | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize;
            const auto viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);

            ImGui::PushFont(ImGuiManager::get_font(Font::Title));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::Begin("MainMenu", nullptr, windowFlags);

            ImGuiTools::TextCentered("DrkCraft");

            ImGui::Dummy({250, 100});

            ImGui::BeginGroup();

            if (ImGuiTools::ButtonCentered("Play", {250, 100}))
                start_game();

            ImGui::Dummy({250, 25});

            if (ImGuiTools::ButtonCentered("Settings", {250, 100}))
                open_settings();

            ImGui::Dummy({250, 25});

            if (ImGuiTools::ButtonCentered("Exit", {250, 100}))
                exit();

            ImGui::EndGroup();

            ImGui::End();
            ImGui::PopFont();
            ImGui::PopStyleVar();
        }
    }

    void MainMenu::on_event(Event& event)
    {
        EventDispatcher ed(event);
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));
    }

    bool MainMenu::on_key_pressed(const KeyPressedEvent& event)
    {
        switch (event.key)
        {
            case KeyCode::Enter:
            {
                start_game();
                return true;
            }
            case KeyCode::Escape:
            {
                exit();
                return true;
            }
            default:
                return false;
        }
    }

    void MainMenu::start_game(void)
    {
        DRK_LOG_CORE_TRACE("MainMenu: Starting Game");
        Application::get_instance().add_layer(Layer::create<Game>());
        detach_layer();
    }

    void MainMenu::open_settings(void)
    {
        DRK_LOG_CORE_TRACE("MainMenu: Opening Settings");
        m_settingsMenu->activate_layer();
        hide();
    }

    void MainMenu::exit(void)
    {
        DRK_LOG_CORE_INFO("MainMenu: Exit Application");
        Application::get_instance().exit();
    }
}
