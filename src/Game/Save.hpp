#ifndef DRK_GAME_WORLD_SAVE_HPP
#define DRK_GAME_WORLD_SAVE_HPP

#include "Core/Base.hpp"
#include "Game/World/World.hpp"
#include "Core/Version.hpp"
#include "Util/Time.hpp"

#include "lib/glm/vec3.hpp"
#include "lib/fs.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>

namespace DrkCraft::Game
{
    struct SaveData // JSON files
    {
        struct Save
        {
            Version version;
            std::string name;
            Time::LocalTime lastSave;
            uint day;
            uint time;

            void to_json(fs::path filename) const;
            static Ptr<Save> from_json(fs::path filename);
        } save;

        struct World
        {
            uint seed;
            // spec
            std::vector<ivec3> chunks;
        } world;

        struct Player
        {
            vec3 location;
            int health;
        } player;
    };

    using SaveInfo = SaveData::Save;

    class SaveLoader
    {
    public:
        SaveLoader(fs::path dir, const SaveInfo& info);
        Ptr<World> load(void);

    private:
        fs::path m_dir;
        SaveInfo m_info;

        Ptr<World> m_world;
    };

    class SaveManager
    {
    public:
        SaveManager(fs::path savesDir);

        void load_save_info(void);
        std::vector<SaveInfo*> get_saves(void) const;

        Error<> rename_save(const std::string& oldName, const std::string& newName);
        Error<> delete_save(const std::string& name);

        Ptr<SaveLoader> get_loader(const std::string& name);

        void update_save_time(const std::string& name, Time::LocalTime time=Time::get_local_time());

    private:
        ErrorMsg validate_save_info(const SaveInfo& info, std::string_view name, const Version& buildVersion);

    private:
        fs::path m_directory;
        std::unordered_map<std::string, Ptr<SaveInfo>> m_saves;
    };
}

#endif // DRK_GAME_WORLD_SAVE_LOADER_HPP
