#include "Yaml.hpp"

#include "Disk/File.hpp"
#include "Core/Debug/Profiler.hpp"

namespace DrkCraft::Yaml
{
    Optional<YAML::Node> load(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();
        DRK_ASSERT_DEBUG(is_file(filename), "YAML file \"{}\" not found", filename.generic_string());

        YAML::Node document;
        bool error = false;
        try
        {
            document = YAML::LoadFile(filename.string());
        }
        catch (const YAML::ParserException&)
        {
            error = true;
        }

        if (!error && !document.IsDefined() || !(document.IsMap() || document.IsSequence()))
            error = true;

        if (error)
        {
            DRK_ASSERT_CORE_FALSE("Could not load YAML file \"{}\"", filename.generic_string());
            return {};
        }
        else
            return { std::move(document) };
    }

    bool check_map(const YAML::Node& node, const string& key)
    {
        return node[key] && node[key].IsMap();
    }

    bool check_scalar(const YAML::Node& node, const string& key)
    {
        return node[key] && node[key].IsScalar();
    }

    Optional<YAML::Node> get_map(const YAML::Node& node, const string& key)
    {
        if (check_map(node, key))
            return node[key];
        else
            return {};
    }

    Optional<YAML::Node> get_scalar(const YAML::Node& node, const string& key)
    {
        if (check_scalar(node, key))
            return node[key];
        else
            return {};
    }
}
