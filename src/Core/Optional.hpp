#ifndef DRK_CORE_OPTIONAL_HPP
#define DRK_CORE_OPTIONAL_HPP

#include <optional>

namespace DrkCraft
{
    template <typename T>
    using Optional = std::optional<T>;

    using std::make_optional;
}

#endif // DRK_CORE_OPTIONAL_HPP
