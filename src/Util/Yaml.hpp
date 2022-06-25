#ifndef DRK_UTIL_YAML_HPP
#define DRK_UTIL_YAML_HPP

#include "Core/Base.hpp"

#include "lib/fs.hpp"
#include "lib/string.hpp"

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

#endif // DRK_UTIL_YAML_HPP
