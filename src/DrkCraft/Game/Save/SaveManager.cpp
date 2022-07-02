#include "SaveManager.hpp"

#include "Util/Json.hpp"
#include "Util/File.hpp"
#include "Core/Debug/Profiler.hpp"

#include <fmt/format.h>

#include <utility>

namespace DrkCraft::Game
{
    ////////////////////////////
    //       SaveLoader       //
    ////////////////////////////

    SaveLoader::SaveLoader(fs::path dir)
      : m_dir(std::move(dir)),
        m_info(*SaveData::Save::from_json(m_dir / "save.json"))
    { }

    SaveLoader::SaveLoader(fs::path dir, const SaveInfo& info)
      : m_dir(std::move(dir)),
        m_info(info)
    { }

    template <>
    Ptr<SaveData> SaveLoader::load(void) const
    {
        DRK_PROFILE_FUNCTION();

        return make_ptr<SaveData>(m_info,
            *SaveData::World::from_json(m_dir / "world.json"),
            *SaveData::Player::from_json(m_dir / "player.json")
        );
    }

    template <>
    Ptr<SaveData::Save> SaveLoader::load(void) const
    {
        DRK_PROFILE_FUNCTION();

        return make_ptr<SaveData::Save>(m_info);
    }

    template <>
    Ptr<SaveData::Player> SaveLoader::load(void) const
    {
        DRK_PROFILE_FUNCTION();

        return SaveData::Player::from_json(m_dir / "player.json");
    }

    template <>
    Ptr<SaveData::World> SaveLoader::load(void) const
    {
        DRK_PROFILE_FUNCTION();

        return SaveData::World::from_json(m_dir / "world.json");
    }

    const fs::path& SaveLoader::dirname(void) const
    {
        return m_dir;
    }

    Ptr<SaveLoader> create_new_save(fs::path dir)
    {
        DRK_ASSERT_DEBUG(!path_exists(dir), "Directory \"{}\" already exists for save: {{}}", dir.generic_string());
        make_dirs(dir);

        return make_ptr<SaveLoader>(std::move(dir), SaveInfo{

        });
    }

    /////////////////////////////
    //       SaveManager       //
    /////////////////////////////

    SaveManager::SaveManager(fs::path savesDir, bool load)
      : m_directory(std::move(savesDir))
    {
        DRK_PROFILE_FUNCTION();

        if (!is_dir(m_directory))
        {
            DRK_LOG_CORE_WARN("Saves directory \"{}\" is not a directory", m_directory.generic_string());
            make_dirs(m_directory);
        }

        if (load)
            load_save_info();
    }

    void SaveManager::load_save_info(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Loading saves from: \"{}\"", m_directory.generic_string());

        m_saves.clear();
        for (const DirEntry& item : DirIterator{m_directory})
            if (item.is_directory())
            {
                const auto& path = item.path();
                if (string name = path.filename().string(); is_dir(name))
                {
                    if (const auto saveJson = path / "save.json"; is_file(saveJson))
                    {
                        auto saveData = SaveData::Save::from_json(saveJson);
                        if (const auto result = validate_save_info(*saveData, name, Build::VERSION); result.has_error())
                        {
                            DRK_LOG_CORE_WARN(result.get_error());
                        }
                        else
                        {
                            m_saves.emplace(name, std::move(saveData));
                        }
                    }
                }
            }
        DRK_LOG_CORE_DEBUG("No more saves");
    }

    std::vector<SaveInfo*> SaveManager::get_save_list(void) const
    {
        DRK_PROFILE_FUNCTION();

        std::vector<SaveInfo*> saves;
        for (const auto& save : m_saves)
        {
            const auto& [name, info] = save;
            if (info)
                saves.push_back(info.get());
        }
        DRK_LOG_CORE_DEBUG("Saves:");
        for (const auto& save : saves)
            DRK_LOG_CORE_DEBUG("- {}", save->name);
        return saves;
    }

    uint SaveManager::count(void) const
    {
        return m_saves.size();
    }

    const fs::path& SaveManager::saves_location(void) const
    {
        return m_directory;
    }

    Error<> SaveManager::rename_save(const string& oldName, const string& newName)
    {
        DRK_PROFILE_FUNCTION();

        DRK_ASSERT_DEBUG(m_saves.contains(oldName), "Save not found");
        if (m_saves.contains(newName) || !m_saves.contains(oldName))
            return Result<>::Failure;

        const fs::path saveJson = m_directory / oldName / "save.json";

        m_saves[newName] = std::move(m_saves[oldName]);
        m_saves[newName]->name = newName;

        if (auto it = m_saves.find(oldName); it != std::end(m_saves))
            m_saves.erase(it);
        else
            return Result<>::Failure;

        fs::rename(m_directory / oldName, m_directory / newName);
        return Result<>::Success;
    }

    Error<> SaveManager::delete_save(const string& name)
    {
        DRK_ASSERT_DEBUG(m_saves.contains(name), "Save not found");

        if (auto it = m_saves.find(name); it != std::end(m_saves))
            m_saves.erase(it);
        else
            return Result<>::Failure;

        fs::remove_all(m_directory / name);
        return Result<>::Success;
    }

    void SaveManager::touch(const string& name, Time::LocalTime time)
    {
        auto& saveInfo = *m_saves[name];
        saveInfo.lastSave = time;

        saveInfo.to_json(m_directory / name / "save.json");
    }

    Ptr<SaveLoader> SaveManager::get_loader(const string& name)
    {
        DRK_PROFILE_FUNCTION();
        DRK_ASSERT_DEBUG(m_saves.contains(name), "Save not found");

        auto savePath = m_directory / name;
        return make_ptr<SaveLoader>(savePath, *m_saves[name]);
    }

    ErrorMsg SaveManager::validate_save_info(const SaveInfo& info, string_view name, const Version& buildVersion)
    {
        if (info.name != name)
            return {
                fmt::format("Invalid save name: \"{}\" (dir name: {})", info.name, name)
            };
        if (info.version != buildVersion)
            return {
                fmt::format("Invalid save version: {} (DrkCraft version: {}", info.version.str(), buildVersion.str())
            };
        return Result<>::Success;
    }
}
