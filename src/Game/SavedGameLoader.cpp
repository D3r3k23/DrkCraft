#include "SavedGameLoader.hpp"

#include "Util/Json.hpp"
#include "Util/File.hpp"
#include "Core/Debug/Profiler.hpp"

#include <rapidjson/document.h>

namespace DrkCraft
{
    std::vector<fs::path> SavedGameLoader::get_saves(const fs::path& savesDir)
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

    ResultStatus SavedGameLoader::rename_save(const fs::path& savesDir, const std::string& oldName, const std::string& newName)
    {
        const fs::path oldSaveJson = savesDir / oldName / "save.json";

        rapidjson::Document save;
        if (Json::parse(oldSaveJson, save))
            save["name"].SetString(newName.c_str(), newName.length());

        Json::write(oldSaveJson, save);

        fs::rename(savesDir / oldName, savesDir / newName);

        return ResultSuccess;
    }

    ResultStatus SavedGameLoader::delete_save(const fs::path& save)
    {
        fs::remove_all(save);
        return ResultSuccess;
    }

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
