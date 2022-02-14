#include "Yaml.hpp"

#include "Util/File.hpp"
#include "Core/Debug/Profiler.hpp"

namespace DrkCraft::Yaml
{
    std::optional<YAML::Node> load(const fs::path& filename)
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

        if (!document.IsDefined() || !(document.IsMap() || document.IsSequence()))
            error = true;

        if (error)
        {
            DRK_ASSERT_CORE(false, "Could not load YAML file \"{}\"", filename.generic_string());
            return {};
        }
        else
            return { std::move(document) };
    }

    bool check_map(const YAML::Node& node, std::string_view key)
    {
        return node[key] && node[key].IsMap();
    }

    bool check_map(const YAML::Node& node, std::string_view key)
    {
        return node[key] && node["key"].IsScalar();
    }
}
