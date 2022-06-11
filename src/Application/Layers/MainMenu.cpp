#include "MainMenu.hpp"

#include "Application/Application.hpp"
#include "Application/ImGuiController.hpp"
#include "Util/ImGui.hpp"
#include "Core/Settings.hpp"
#include "Game/SaveLoader.hpp"
#include "Game/Layers/GameLayer.hpp"
#include "Util/Fn.hpp"
#include "Core/Debug/Profiler.hpp"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <string>
#include <algorithm>

namespace DrkCraft
{
    using namespace DrkCraft::Game;

    static const AssetList s_REQUIRED_ASSETS
    {

    };

    const AssetList& MainMenu::get_asset_list(void)
    {
        return s_REQUIRED_ASSETS;
    }

    MainMenu::MainMenu(void)
      : Layer("MainMenuLayer", true),
        m_settingsMenu(Layer::create<SettingsMenu>(false)),
        m_loadingScreen(Layer::create<LoadingScreen>("Loading Assets")),
        m_saves(SaveLoader::get_saves("saves")),
        m_show(true)
    {
        DRK_PROFILE_FUNCTION();

        m_settingsMenu->set_close_callback_fn(DRK_BIND_FN(show_menu));
        {
            DRK_PROFILE_SCOPE("Sort saves");
            std::ranges::sort(m_saves, [](const auto& s1, const auto& s2)
            {
                return fs::last_write_time(s1 / "save.json") < fs::last_write_time(s2 / "save.json");
            });
        }
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
        Application::add_layer(m_settingsMenu);
    }

    void MainMenu::on_detach(void)
    {
        m_settingsMenu->detach_layer();
    }

    void MainMenu::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        // Show background

        if (m_show)
        {
            DrkImGui::BeginFullscreen("Main Menu", ImGuiWindowFlags_NoBackground);

            // Title image

            ImGui::Dummy({250, 100});
            ImGui::PushFont(ImGuiController::get_font(ImGuiFont::Title));
            DrkImGui::TextCentered("DrkCraft");
            ImGui::PopFont();

            ImGui::Dummy({250, 50});
            ImGui::BeginGroup();
            ImGui::PushFont(ImGuiController::get_font(ImGuiFont::Button));

            static bool showStartGameMenu = false;
            if (DrkImGui::ButtonCentered("Play", {250, 100}))
            {
                showStartGameMenu = true;
                ImGui::OpenPopup("Start Game");
            }
            if (ImGui::BeginPopupModal("Start Game", &showStartGameMenu))
            {
                // show_saved_games_table();

                if (ImGui::Button("Create New World"))
                {
                    ImGui::OpenPopup("Create World");
                }
                if (ImGui::BeginPopup("Create World"))
                {
                    show_create_world_menu();
                    ImGui::EndPopup();
                }
                ImGui::EndPopup();
            }

            ImGui::Dummy({250, 25});

            if (DrkImGui::ButtonCentered("Settings", {250, 100}))
                open_settings();

            ImGui::Dummy({250, 25});

            if (DrkImGui::ButtonCentered("Exit", {250, 100}))
                exit();

            ImGui::EndGroup();
            ImGui::PopFont();

            ImGui::End();
        }
    }

    void MainMenu::on_update(Timestep timestep)
    {
        // Animate background

        attempt_to_start_game();
    }

    void MainMenu::on_event(Event& event)
    {
        EventDispatcher ed(event);
    }

    void MainMenu::show_saved_games_table(void)
    {
        static constexpr ImGuiTableFlags flags = ImGuiTableFlags_NoBordersInBody
          | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter
          | ImGuiTableFlags_BordersV  | ImGuiTableFlags_ScrollY;

        static constexpr uint NUM_SAVES = 10;
        const float textHeight = ImGui::GetTextLineHeightWithSpacing();

        static const fs::path savesDir{"saves"};

        if (ImGui::BeginTable("Saves", 3, flags, {0.0f, textHeight * NUM_SAVES}));
        {
            ImGui::TableSetupColumn("Save");
            ImGui::TableSetupColumn("Last Played");
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableHeadersRow();

            ImGuiListClipper clipper;
            clipper.Begin(m_saves.size());
            while (clipper.Step())
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {
                    auto& save = m_saves[row];
                    auto saveName = save.filename().string();
                    ImGui::TableNextRow();

                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(saveName.c_str());

                    ImGui::TableNextColumn();
                    if (ImGui::SmallButton("Start"))
                    {
                        m_gameLoadSource = save;
                    }

                    ImGui::TableNextColumn();
                    if (ImGui::SmallButton("Edit"))
                    {
                        ImGui::OpenPopup("Edit Save");
                    }
                    if (ImGui::BeginPopup("Edit Save"))
                    {
                        std::string name;
                        if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            if (SaveLoader::rename_save(savesDir, saveName, name) == Result::Success)
                                save = savesDir / name;
                        }
                        ImGui::EndPopup();
                    }

                    ImGui::TableNextColumn();
                    if (ImGui::SmallButton("Delete"))
                    {
                        ImGui::OpenPopup("Delete Save");
                    }
                    if (ImGui::BeginPopupModal("Delete Save"))
                    {
                        ImGui::Text("Are you sure you want to delete saved game \"%s\"?", saveName);
                        if (ImGui::Button("YES"))
                        {
                            SaveLoader::delete_save(save);
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("CANCEL"))
                        {
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                }
            ImGui::EndTable();
        }
    }

    void MainMenu::show_create_world_menu(void)
    {
        WorldGeneratorSpec worldGeneratorSpec;

        m_gameLoadSource = worldGeneratorSpec;
    }

    void MainMenu::attempt_to_start_game(void)
    {
        if (!std::holds_alternative<std::monostate>(m_gameLoadSource))
        {
            if (!Application::get_asset_library().loading())
            {
                start_game();
            }
            else if (!m_loadingScreen)
            {
                DRK_LOG_CORE_INFO("Game selected to load, waiting for assets to load");
                Application::add_layer(m_loadingScreen);
            }
        }
    }

    void MainMenu::start_game(void)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("MainMenu: Starting Game");
        DRK_ASSERT_DEBUG(!std::holds_alternative<std::monostate>(m_gameLoadSource), "Game has not been selected");

        Visitor
        {
            [this](const fs::path& save)
            {
                Application::add_layer(Layer::create<GameLayer>(std::move(m_loadingScreen), save));
            },
            [this](const WorldGeneratorSpec& worldGeneratorSpec)
            {
                Application::add_layer(Layer::create<GameLayer>(std::move(m_loadingScreen), worldGeneratorSpec));
            },
            [](std::monostate){ DRK_ASSERT_DEBUG(false, "Invalid variant"); }
        }.visit(m_gameLoadSource);

        detach_layer();
    }

    void MainMenu::open_settings(void)
    {
        DRK_LOG_CORE_TRACE("MainMenu: Opening Settings");
        m_settingsMenu->open();
        hide_menu();
    }

    void MainMenu::exit(void)
    {
        DRK_LOG_CORE_INFO("MainMenu: Exit Application");
        Application::exit();
    }
}
