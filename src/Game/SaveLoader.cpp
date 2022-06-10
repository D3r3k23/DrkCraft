#include "SaveLoader.hpp"

#include "Util/Json.hpp"
#include "Util/File.hpp"
#include "Core/Debug/Profiler.hpp"

#include <rapidjson/document.h>

namespace DrkCraft::Game
{
    std::vector<fs::path> SaveLoader::get_saves(const fs::path& savesDir)
    {
        DRK_PROFILE_FUNCTION();
        DRK_ASSERT_DEBUG(is_dir(savesDir), "Saves directory \"{}\" is not a directory", savesDir.generic_string());

        std::vector<fs::path> saves;
        for (const DirEntry& item : DirIterator{savesDir})
        {
            if (item.is_directory())
            {
                const auto& path = item.path();
                if (is_file(path / "save.json"))
                    saves.push_back(path);
            }
        }
        return saves;
    }

    Result SaveLoader::rename_save(const fs::path& savesDir, const std::string& oldName, const std::string& newName)
    {
        const fs::path oldSaveJson = savesDir / oldName / "save.json";

        rapidjson::Document save;
        if (Json::parse(oldSaveJson, save))
            save["name"].SetString(newName.c_str(), newName.length());
        else
            return Result::Failure;

        Json::write(oldSaveJson, save);

        fs::rename(savesDir / oldName, savesDir / newName);

        return Result::Success;
    }

    Result SaveLoader::delete_save(const fs::path& save)
    {
        fs::remove_all(save);
        return Result::Success;
    }

    SaveLoader::SaveLoader(const fs::path& dir)
      : m_dir(dir)
    { }

    Ptr<World> SaveLoader::load(void)
    {
        DRK_PROFILE_FUNCTION();

        m_world = make_ptr<World>();

        return std::move(m_world);
    }
}
