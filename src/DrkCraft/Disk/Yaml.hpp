#ifndef DRK_DISK_YAML_HPP
#define DRK_DISK_YAML_HPP

#include "Core/Base.hpp"

#include "Lib/fs.hpp"
#include "Lib/string.hpp"

#include <yaml-cpp/yaml.h>

namespace DrkCraft::Yaml
{
    Optional<YAML::Node> load(const fs::path& filename);

    bool check_map(const YAML::Node& node, const string& key);
    bool check_scalar(const YAML::Node& node, const string& key);

    Optional<YAML::Node> get_map(const YAML::Node& node, const string& key);
    Optional<YAML::Node> get_scalar(const YAML::Node& node, const string& key);

    template <typename T>
    Optional<T> get_value(const YAML::Node& node)
    {
        try
        {
            return node.as<T>();
        }
        catch (const YAML::BadConversion&)
        {
            return {};
        }
    }
}

#endif // DRK_DISK_YAML_HPP
