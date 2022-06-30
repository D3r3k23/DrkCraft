#ifndef DRK_GAME_SAVE_SAVE_MANAGER_HPP
#define DRK_GAME_SAVE_SAVE_MANAGER_HPP

#include "Core/Base.hpp"
#include "Game/Save/SaveData.hpp"
#include "Game/World/World.hpp"
#include "Core/Version.hpp"
#include "Util/Time.hpp"

#include "lib/glm/vec3.hpp"

#include "lib/fs.hpp"
#include "lib/string.hpp"
#include "lib/string_view.hpp"

#include <vector>
#include <unordered_map>

namespace DrkCraft::Game
{
    class SaveLoader
    {
    public:
        SaveLoader(fs::path dir);
        SaveLoader(fs::path dir, const SaveInfo& info);

        template <typename SD>
        Ptr<SD> load(void) const;
        const fs::path& dirname(void) const;

    private:
        fs::path m_dir;
        SaveInfo m_info;

        Ptr<World> m_world;
    };

    template <> Ptr<SaveData> SaveLoader::load(void) const;
    template <> Ptr<SaveData::Save> SaveLoader::load(void) const;
    template <> Ptr<SaveData::Player> SaveLoader::load(void) const;
    template <> Ptr<SaveData::World> SaveLoader::load(void) const;

    Ptr<SaveLoader> create_new_save(fs::path dir);

    class SaveManager // For managing existing saves
    {
    public:
        SaveManager(fs::path savesDir, bool load=true);

        void load_save_info(void);
        std::vector<SaveInfo*> get_save_list(void) const;

        uint count(void) const;
        const fs::path& saves_location(void) const;

        Error<> rename_save(const string& oldName, const string& newName);
        Error<> delete_save(const string& name);

        void touch(const string& name, Time::LocalTime time=Time::get_local_time());

        Ptr<SaveLoader> get_loader(const string& name);

    private:
        ErrorMsg validate_save_info(const SaveInfo& info, string_view name, const Version& buildVersion);

    private:
        fs::path m_directory;
        std::unordered_map<string, Ptr<SaveInfo>> m_saves;
    };
}

#endif // DRK_GAME_SAVE_SAVE_MANAGER_HPP
