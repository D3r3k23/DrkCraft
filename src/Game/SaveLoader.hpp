#ifndef DRK_GAME_WORLD_SAVE_LOADER_HPP
#define DRK_GAME_WORLD_SAVE_LOADER_HPP

#include "Core/Base.hpp"
#include "Game/World/World.hpp"

#include "lib/fs.hpp"

#include <vector>
#include <string>

namespace DrkCraft::Game
{
    class SaveLoader
    {
    public:
        static std::vector<fs::path> get_saves(const fs::path& savesDir);
        static Result rename_save(const fs::path& savesDir, const std::string& oldName, const std::string& newName);
        static Result delete_save(const fs::path& save);

        SaveLoader(const fs::path& dir);
        Ptr<World> load(void);

    private:
        fs::path m_dir;
        Ptr<World> m_world;
    };
}

#endif // DRK_GAME_WORLD_SAVE_LOADER_HPP
