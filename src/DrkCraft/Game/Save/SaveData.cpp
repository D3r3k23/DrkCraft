#include "SaveData.hpp"

#include "Util/Json.hpp"
#include "Util/File.hpp"
#include "Core/Debug/Profiler.hpp"

#include <fmt/format.h>

#include <utility>

namespace DrkCraft::Game
{
    //////////////////////////
    //       SaveData       //
    //////////////////////////

    void SaveData::Save::to_json(fs::path filename) const
    {
        Json::FileWriter<Json::PrettyWriter> writer(std::move(filename));
        {
            auto& w = writer.get_writer();
            w.StartObject();
                w.Key("drkcraft_version"); w.String(string(version.string()));
                w.Key("name");             w.String(name);
                w.Key("last_save");        w.Int(lastSave.time_since_epoch().count());
                w.Key("day");              w.Uint(day);
                w.Key("time");             w.Uint(time);
            w.EndObject();
        }
    }

    Ptr<SaveData::Save> SaveData::Save::from_json(fs::path filename)
    {
        const auto document = Json::parse(std::move(filename));
        const auto& save = *document;
        return make_ptr<SaveData::Save>(
            Version(save["drkcraft_version"].GetString()),
            save["name"].GetString(),
            Time::LocalTime(Time::Seconds<int>(save["last_save"].GetInt())),
            save["day"].GetUint(),
            save["time"].GetUint()
        );
    }

    void SaveData::Player::to_json(fs::path filename) const
    {
        Json::FileWriter<Json::PrettyWriter> writer(std::move(filename));
        {
            auto& w = writer.get_writer();
            w.StartObject();
                w.Key("location"); w.StartObject();
                    w.Key("x"); w.Double(location.x);
                    w.Key("y"); w.Double(location.y);
                    w.Key("z"); w.Double(location.z);
                w.EndObject();
                w.Key("health");   w.Int(health);
            w.EndObject();
        }
    }

    Ptr<SaveData::Player> SaveData::Player::from_json(fs::path filename)
    {
        const auto document = Json::parse(std::move(filename));
        const auto& save = *document;
        return make_ptr<SaveData::Player>(
            vec3(
                save["location"]["x"].GetInt(),
                save["location"]["y"].GetInt(),
                save["location"]["z"].GetInt()
            ),
            save["health"].GetInt()
        );
    }

    void SaveData::World::to_json(fs::path filename) const
    {
        Json::FileWriter<Json::PrettyWriter> writer(std::move(filename));
        {
            auto& w = writer.get_writer();
            w.StartObject();
                w.Key("seed");   w.Uint(seed);
                w.Key("chunks"); w.StartArray();
                for (const auto& chunk : chunks)
                {
                    w.StartObject();
                        w.Key("x"); w.Int(chunk.x);
                        w.Key("y"); w.Int(chunk.y);
                        w.Key("z"); w.Int(chunk.z);
                    w.EndObject();
                }
                w.EndArray();
            w.EndObject();
        }
    }

    Ptr<SaveData::World> SaveData::World::from_json(fs::path filename)
    {
        const auto document = Json::parse(std::move(filename));
        const auto& save = *document;
        auto world = make_ptr<SaveData::World>(
            save["seed"].GetUint()
        );
        for (const auto chunks = save["chunks"].GetArray(); const auto& chunk : chunks)
        {
            world->chunks.push_back(
            {
                chunk["x"].GetInt(),
                chunk["y"].GetInt(),
                chunk["z"].GetInt()
            });
        }
    }

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
            SaveData::World::from_json(m_dir / "world.json"),
            SaveData::Player::from_json(m_dir / "player.json")
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
}
