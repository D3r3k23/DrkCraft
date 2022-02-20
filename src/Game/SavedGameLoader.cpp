#include "SavedGameLoader.hpp"

#include "Core/Debug/Profiler.hpp"

namespace DrkCraft
{
    SavedGameLoader::SavedGameLoader(const fs::path& dir)
      : m_dir(dir)
    { }

    Ptr<World> SavedGameLoader::load(void)
    {
        DRK_PROFILE_FUNCTION();

        m_world = make_ptr<World>();

        return std::move(m_world);
    }
}
