#ifndef DRK_APPLICATION_LAYERS_MAIN_MENU_HPP
#define DRK_APPLICATION_LAYERS_MAIN_MENU_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Util/Timestep.hpp"

#include "Application/Layers/SettingsMenu.hpp"
#include "Application/Layers/LoadingScreen.hpp"
#include "System/AssetLibrary.hpp"
#include "Game/World/WorldGenerator.hpp"

#include "lib/fs.hpp"

#include <vector>
#include <variant>

namespace DrkCraft
{
    class MainMenu : public Layer
    {
    public:
        static const AssetList& get_asset_list(void);

        MainMenu(void);
        virtual ~MainMenu(void);

        void show_menu(void);
        void hide_menu(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_render(void) override;
        virtual void on_update(Timestep timestep) override;
        virtual void on_event(Event& event) override;

    private:
        void show_saved_games_table(void);
        void show_create_world_menu(void);

        void attempt_to_start_game(void);
        void start_game(void);

        void open_settings(void);
        void exit(void);

    private:
        Ref<SettingsMenu> m_settingsMenu;
        Ref<LoadingScreen> m_loadingScreen;

        std::vector<fs::path> m_saves;

        std::variant<std::monostate, fs::path, Game::WorldGeneratorSpec> m_gameLoadSource;

        bool m_show;
    };
}

#endif // DRK_APPLICATION_LAYERS_MAIN_MENU_HPP
