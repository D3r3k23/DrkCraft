#ifndef DRK_UTIL_YAML_HPP
#define DRK_UTIL_YAML_HPP

#include "Core/Base.hpp"

#include "lib/fs.hpp"

#include <yaml-cpp/yaml.h>

#include <string>
#include <optional>

namespace DrkCraft::Yaml
{
    std::optional<YAML::Node> load(const fs::path& filename);

    bool check_map(const YAML::Node& node, const std::string& key);
    bool check_scalar(const YAML::Node& node, const std::string& key);

    std::optional<YAML::Node> get_map(const YAML::Node& node, const std::string& key);
    std::optional<YAML::Node> get_scalar(const YAML::Node& node, const std::string& key);

    template <typename T>
    std::optional<T> get_value(const YAML::Node& node)
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
