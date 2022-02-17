#ifndef DRK_GAME_WORLD_SAVED_WORLD_LOADER_HPP
#define DRK_GAME_WORLD_SAVED_WORLD_LOADER_HPP

#include "Core/Base.hpp"
#include "Game/World/World.hpp"

#include "lib/fs.hpp"

namespace DrkCraft
{
    class SavedWorldLoader
    {
    public:
        SavedWorldLoader(const fs::path& dir);
        Ptr<World> load(void);

    private:
        fs::path m_dir;
        Ptr<World> m_world;
    };
}

#endif // DRK_GAME_WORLD_SAVED_WORLD_LOADER_HPP
