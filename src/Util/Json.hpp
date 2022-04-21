#ifndef DRK_UTIL_JSON_HPP
#define DRK_UTIL_JSON_HPP

#include "Core/Base.hpp"

#include "lib/fs.hpp"

#include <rapidjson/document.h>

namespace DrkCraft::Json
{
    Result parse(const fs::path& filename, rapidjson::Document& document);
    void write(const fs::path& filename, const rapidjson::Document& document);
}

#endif // DRK_UTIL_JSON_HPP
