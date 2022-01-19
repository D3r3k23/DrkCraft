#include "MainMenu.hpp"

#include "Application.hpp"
#include "ImGuiTools.hpp"
#include "Game/Game.hpp"
#include "System/AssetManager.hpp"
#include "Core/Profiler.hpp"

#include <imgui/imgui.h>

namespace DrkCraft
{
    MainMenu::MainMenu(void)
      : Layer("MainMenuLayer", true),
        m_show(true),
        m_applicationAssetsLoading(true),
        m_startButtonPushed(false)
    {
        DRK_PROFILE_FUNCTION();

        m_settingsMenu = Layer::create<SettingsMenu>(false);
        m_settingsMenu->set_close_callback_fn(DRK_BIND_FN(show_menu));

        m_loadingScreen = Layer::create<LoadingScreen>();
    }

    MainMenu::~MainMenu(void)
    {

    }

    void MainMenu::show_menu(void)
    {
        m_show = true;
    }

    void MainMenu::hide_menu(void)
    {
        m_show = false;
    }

    void MainMenu::on_attach(void)
    {
        Application::get_instance().add_layer(m_settingsMenu);
    }

    void MainMenu::on_detach(void)
    {
        m_settingsMenu->detach_layer();
    }

    void MainMenu::on_update(Timestep timestep)
    {
        // Animate background?

        if (m_applicationAssetsLoading && !Application::get_assets().busy() && m_startButtonPushed)
        {
            m_loadingScreen->detach_layer();
            start_game();
        }
        else
            m_applicationAssetsLoading = Application::get_assets().busy();
    }

    void MainMenu::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        // Show background

        if (m_show)
        {
            ImGuiTools::BeginFullscreen("Main Menu", ImGuiWindowFlags_NoBackground);

            ImGui::Dummy({250, 100});
            ImGui::PushFont(ImGuiManager::get_font(ImGuiFont::Title));
            ImGuiTools::TextCentered("DrkCraft");
            ImGui::PopFont();

            ImGui::Dummy({250, 50});
            ImGui::BeginGroup();
            ImGui::PushFont(ImGuiManager::get_font(ImGuiFont::Button));

            if (ImGuiTools::ButtonCentered("Play", {250, 100}))
                start_game();

            ImGui::Dummy({250, 25});

            if (ImGuiTools::ButtonCentered("Settings", {250, 100}))
                open_settings();

            ImGui::Dummy({250, 25});

            if (ImGuiTools::ButtonCentered("Exit", {250, 100}))
                exit();

            ImGui::EndGroup();
            ImGui::PopFont();

            ImGui::End();
        }
    }

    void MainMenu::on_event(Event& event)
    {
        EventDispatcher ed(event);
    }

    void MainMenu::start_game(void)
    {
        if (Application::get_assets().busy())
        {
            DRK_LOG_CORE_INFO("Waiting for assets to finish loading");
            Application::get_instance().add_layer(m_loadingScreen);
            hide_menu();
            m_startButtonPushed = true;
        }
        else
        {
            DRK_LOG_CORE_TRACE("MainMenu: Starting Game");
            Application::get_instance().add_layer(Layer::create<Game>());
            detach_layer();
        }
    }

    void MainMenu::open_settings(void)
    {
        DRK_LOG_CORE_TRACE("MainMenu: Opening Settings");
        m_settingsMenu->activate_layer();
        hide_menu();
    }

    void MainMenu::exit(void)
    {
        DRK_LOG_CORE_INFO("MainMenu: Exit Application");
        Application::get_instance().exit();
    }
}
