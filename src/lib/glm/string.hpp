#ifndef DRK_LIB_GLM_STRING_HPP
#define DRK_LIB_GLM_STRING_HPP

#include "lib/glm/vec.hpp"
#include "lib/glm/mat.hpp"

#include <glm/gtx/string_cast.hpp>
#include <fmt/format.h>

#include <string>

namespace DrkCraft
{
    template <typename V>
    std::string to_string(const V& vec)
    {
        return glm::to_string(vec);
    }
}

template <typename V>
struct fmt::formatter<V>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const V& vec, FormatContext& ctx)
    {
        return format_to(ctx.out(), "%s", to_string(vec))
    }
};

#endif // DRK_LIB_GLM_STRING_HPP
