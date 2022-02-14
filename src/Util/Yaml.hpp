#ifndef DRK_UTIL_YAML_HPP
#define DRK_UTIL_YAML_HPP

#include "Core/Base.hpp"

#include "lib/fs.hpp"

#include <yaml-cpp/yaml.h>

#include <string_view>
#include <optional>

namespace DrkCraft::Yaml
{
    std::optional<YAML::Node> load(const fs::path& filename);

    bool check_map(const YAML::Node& node, std::string_view key);
    bool check_scalar(const YAML::Node& node, std::string_view key);
}

#endif // DRK_UTIL_YAML_HPP
