#ifndef DRK_APPLICATION_LAYERS_MAIN_MENU_HPP
#define DRK_APPLICATION_LAYERS_MAIN_MENU_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Util/Timestep.hpp"

#include "System/Library.hpp"
#include "Application/Layers/SettingsMenu.hpp"
#include "Application/Layers/LoadingScreen.hpp"
#include "Game/Layers/GameLayer.hpp"
#include "Game/Save/SaveViewer.hpp"
#include "Game/World/WorldGenerator.hpp"
#include "System/Thread.hpp"

#include "Lib/fs.hpp"
#include "Lib/string.hpp"
#include "Lib/string_view.hpp"

#include <atomic>

namespace DrkCraft
{
    class MainMenu : public Layer
    {
    public:
        MainMenu(void);
        virtual ~MainMenu(void);

        void show_menu(void);
        void hide_menu(void);
        bool shown(void) const;
        bool background_shown(void) const;

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_render(void) override;
        virtual void on_update(Timestep timestep) override;
        virtual void on_event(Event& event) override;

    private:
        void show_saved_games_table(void);
        void show_create_world_menu(void);

        void load_save_list(void);
        void load_save(const string& saveName);
        void create_save(string_view newSaveName, const Game::WorldGeneratorSpec& worldSpec);
        void launch_game(void);
        void open_settings(void);
        void exit(void);

    private:
        Library& m_library;

        Ref<LoadingScreen> m_loadingScreen;
        Ref<SettingsMenu> m_settingsMenu;
        Ref<Game::GameLayer> m_gameLayer;

        Game::SaveManager m_saveManager;
        std::vector<Game::SaveInfo*> m_saveList;
        Thread<> m_saveLoadThread;

        bool m_assetsLoading;
        bool m_savesLoading;

        Ptr<AssetLoadToken> m_assetLoadToken;

        bool m_show;
        bool m_showBackground;
    };
}

#endif // DRK_APPLICATION_LAYERS_MAIN_MENU_HPP
