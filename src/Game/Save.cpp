#include "Save.hpp"

#include "Util/Json.hpp"
#include "Util/File.hpp"
#include "Core/Debug/Profiler.hpp"

#include <fmt/format.h>

#include <utility>

namespace DrkCraft::Game
{
    void SaveData::Save::to_json(fs::path filename) const
    {
        Json::FileWriter<Json::PrettyWriter> writer(std::move(filename));
        {
            auto& w = writer.get_writer();
            w.Key("drkcraft_version"); w.String(std::string(version.string()));
            w.Key("name");             w.String(name);
            w.Key("last_save");        w.Int(lastSave.time_since_epoch().count());
            w.Key("day");              w.Uint(day);
            w.Key("time");             w.Uint(time);
        }
    }

    Ptr<SaveData::Save> SaveData::Save::from_json(fs::path filename)
    {
        const auto document = Json::parse(std::move(filename));
        const auto& save = *document;
        return Ptr<SaveData::Save>(new SaveData::Save
        {
            { save["drkcraft_version"].GetString() },
            save["name"].GetString(),
            Time::LocalTime(Time::Seconds<int>(save["last_save"].GetInt())),
            save["day"].GetUint(),
            save["time"].GetUint()
        });
    }

    SaveLoader::SaveLoader(fs::path dir, const SaveInfo& info)
      : m_dir(std::move(dir)),
        m_info(info)
    { }

    Ptr<World> SaveLoader::load(void)
    {
        DRK_PROFILE_FUNCTION();

        m_world = make_ptr<World>();

        return std::move(m_world);
    }

    SaveManager::SaveManager(fs::path savesDir)
      : m_directory(std::move(savesDir))
    {
        if (!is_dir(m_directory))
        {
            DRK_LOG_CORE_WARN("Saves directory \"{}\" is not a directory", m_directory.generic_string());
            make_dirs(m_directory);
        }

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
                if (std::string name = path.filename().string(); is_dir(name))
                {
                    if (const auto saveJson = path / "save.json"; is_file(saveJson))
                    {
                        auto saveData = SaveData::Save::from_json(saveJson);
                        if (const auto result = validate_save_info(*saveData, name, DRK_BUILD_VERSION); result.has_error())
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

    std::vector<SaveInfo*> SaveManager::get_saves(void) const
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

    Error<> SaveManager::rename_save(const std::string& oldName, const std::string& newName)
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

    Error<> SaveManager::delete_save(const std::string& name)
    {
        DRK_ASSERT_DEBUG(m_saves.contains(name), "Save not found");

        if (auto it = m_saves.find(name); it != std::end(m_saves))
            m_saves.erase(it);
        else
            return Result<>::Failure;

        fs::remove_all(m_directory / name);
        return Result<>::Success;
    }

    Ptr<SaveLoader> SaveManager::get_loader(const std::string& name)
    {
        DRK_PROFILE_FUNCTION();
        DRK_ASSERT_DEBUG(m_saves.contains(name), "Save not found");

        auto savePath = m_directory / name;

        auto loader = make_ptr<SaveLoader>(savePath, *m_saves[name]);

        return loader;
    }

    void SaveManager::update_save_time(const std::string& name, Time::LocalTime time)
    {
        auto& saveInfo = *m_saves[name];
        saveInfo.lastSave = time;

        saveInfo.to_json(m_directory / name / "save.json");
    }

    ErrorMsg SaveManager::validate_save_info(const SaveInfo& info, std::string_view name, const Version& buildVersion)
    {
        if (info.name != name)
            return { fmt::format("Invalid save name: \"{}\" (dir name: {})", info.name, name) };
        if (info.version != buildVersion)
            return {
                fmt::format("Invalid save version: {} (DrkCraft version: {}", info.version.string(), buildVersion.string())
            };
        return Result<>::Success;
    }
}
