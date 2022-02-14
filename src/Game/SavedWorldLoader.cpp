#include "SavedWorldLoaded.hpp"

#include "Core/Debug/Profiler.hpp"

namespace DrkCraft
{
    SavedWorldLoader::SavedWorldLoader(const fs::path& dir)
      : m_dir(dir)
    { }

    Ptr<World> SavedWorldLoader::load(void)
    {
        DRK_PROFILE_FUNCTION();

        m_world = make_ptr<World>();

        return std::move(m_world);
    }
}
