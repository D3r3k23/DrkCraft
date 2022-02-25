#ifndef DRK_GAME_WORLD_SAVED_GAME_LOADER_HPP
#define DRK_GAME_WORLD_SAVED_GAME_LOADER_HPP

#include "Core/Base.hpp"
#include "Game/World/World.hpp"

#include "lib/fs.hpp"

#include <vector>
#include <string>

namespace DrkCraft
{
    class SavedGameLoader
    {
    public:
        static std::vector<fs::path> get_saves(const fs::path& savesDir);
        static ResultStatus rename_save(const fs::path& savesDir, const std::string& oldName, const std::string& newName);
        static ResultStatus delete_save(const fs::path& save);

        SavedGameLoader(const fs::path& dir);
        Ptr<World> load(void);

    private:
        fs::path m_dir;
        Ptr<World> m_world;
    };
}

#endif // DRK_GAME_WORLD_SAVED_GAME_LOADER_HPP
