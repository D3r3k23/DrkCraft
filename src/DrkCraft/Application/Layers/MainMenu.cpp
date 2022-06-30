#include "MainMenu.hpp"

#include "Application/Application.hpp"
#include "Application/ImGuiController.hpp"
#include "System/Thread.hpp"
#include "Util/ImGui.hpp"
#include "Core/Settings.hpp"
#include "Core/Debug/Profiler.hpp"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <algorithm>

namespace DrkCraft
{
    static const fs::path SAVE_DIRECTORY = "data/saves";
    static constexpr bool SHOW_MENU_BACKGROUND_WHEN_HIDDEN = true;

    MainMenu::MainMenu(void)
      : Layer("MainMenuLayer"),
        m_assetLibrary(Application::get_asset_library()),
        m_settingsMenu(Layer::create<SettingsMenu>(false)),
        m_saveManager(SAVE_DIRECTORY, false),
        m_assetsLoading(false),
        m_savesLoading(false),
        m_show(true),
        m_showBackground(false)
    {
        DRK_PROFILE_FUNCTION();

        m_settingsMenu->set_close_callback_fn(DRK_BIND_FN(show_menu));

        m_loadingScreen = Layer::create<LoadingScreen>("Loading Menu");
        Application::add_overlay(m_loadingScreen);

        m_assetsLoading = true;
        m_assetLoadToken = m_assetLibrary.load_list_and_get_token({});

        m_savesLoading = true;
        m_saveLoadThread = { "save_load_thread", DRK_BIND_FN(load_save_list()) };

        // This adds Game assets to the load queue
        //
        m_gameLayer = Layer::create<Game::GameLayer>(SAVE_DIRECTORY);
    }

    MainMenu::~MainMenu(void)
    {

    }

    void MainMenu::show_menu(void)
    {
        m_show = true;

        if (!m_showBackground)
            m_showBackground = true;
    }

    void MainMenu::hide_menu(void)
    {
        m_show = false;

        if constexpr (!SHOW_MENU_BACKGROUND_WHEN_HIDDEN)
            m_showBackground = false;
    }

    bool MainMenu::shown(void) const
    {
        return m_show;
    }

    bool MainMenu::background_shown(void) const
    {
        return m_showBackground;
    }

    void MainMenu::on_attach(void)
    {
        Application::add_layer(m_settingsMenu);
    }

    void MainMenu::on_detach(void)
    {
        DRK_PROFILE_FUNCTION();

        m_settingsMenu->detach_layer();

        if (m_loadingScreen->is_layer_attached())
            m_loadingScreen->detach_layer();
    }

    void MainMenu::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        if (background_shown())
        {
            // Show background
        }

        if (shown())
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
        DRK_PROFILE_FUNCTION();

        static const auto is_loading = [&]
        {
            return m_assetsLoading || m_savesLoading;
        };

        const bool loading = is_loading();
        if (m_assetsLoading)
        {
            if (m_assetLoadToken->complete())
            {
                m_assetsLoading = false;

                m_showBackground = !m_savesLoading || SHOW_MENU_BACKGROUND_WHEN_HIDDEN;
            }
        }
        if (m_savesLoading)
        {
            if (m_saveLoadThread.stopped())
            {
                m_savesLoading = false;
            }
        }

        const bool wasLoading = loading;
        const bool nowLoading = is_loading();
        if (const bool justFinished = !nowLoading && wasLoading; justFinished)
        {
            m_loadingScreen->detach_layer();
            show_menu();
        }
        if (!nowLoading)
        {
            if (background_shown())
            {
                // Animate background
            }
        }
    }

    void MainMenu::on_event(Event& event)
    {
        EventDispatcher ed(event);
    }

    void MainMenu::show_saved_games_table(void)
    {
        DRK_PROFILE_FUNCTION();

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
            clipper.Begin(m_saveList.size());
            while (clipper.Step())
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {
                    auto& save = m_saveList[row];
                    ImGui::TableNextRow();

                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(save->name.c_str());

                    ImGui::TableNextColumn();
                    if (ImGui::SmallButton("Start"))
                    {
                        if (m_gameLayer->ready_to_load_game())
                        {
                            load_save(save->name);
                            launch_game();
                        }
                    }

                    ImGui::TableNextColumn();
                    if (ImGui::SmallButton("Edit"))
                    {
                        ImGui::OpenPopup("Edit Save");
                    }
                    if (ImGui::BeginPopup("Edit Save"))
                    {
                        string name;
                        if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            if (m_saveManager.rename_save(save->name, name))
                                save->name = name;
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
                        ImGui::Text("Are you sure you want to delete saved game \"%s\"?", save->name);
                        if (ImGui::Button("YES"))
                        {
                            m_saveManager.delete_save(save->name);
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
        Game::WorldGeneratorSpec worldGeneratorSpec;

        // m_gameLoadSource = worldGeneratorSpec;
    }

    void MainMenu::load_save_list(void)
    {
        DRK_PROFILE_FUNCTION();

        m_saveManager.load_save_info(); // This could be done on initialization

        m_saveList = m_saveManager.get_save_list();
        {
            DRK_PROFILE_SCOPE("Sort saves");
            std::ranges::sort(m_saveList, [](const auto& s1, const auto& s2)
            {
                return s1->lastSave < s2->lastSave;
            });
        }

        DRK_LOG_CORE_INFO("MainMenu finished loading saves");
    }

    void MainMenu::load_save(const string& saveName)
    {
        DRK_PROFILE_FUNCTION();

        auto saveLoader = m_saveManager.get_loader(saveName);
        m_gameLayer->load_game(std::move(saveLoader));
    }

    void MainMenu::create_save(string_view newSaveName, const Game::WorldGeneratorSpec& worldSpec)
    {
        DRK_PROFILE_FUNCTION();

        m_gameLayer->load_game(newSaveName, worldSpec);
    }

    void MainMenu::launch_game(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("MainMenu: Opening Game");

        m_gameLayer->attach_layer();
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
