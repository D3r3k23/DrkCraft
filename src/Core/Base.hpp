#ifndef BASE_HPP
#define BASE_HPP

#include "Log.hpp"

#include <memory>
#include <cstdint>

namespace DrkCraft
{
    using uint = unsigned int;
    using uint32 = uint32_t;
    using int32 = int32_t;

    template <typename T>
    using Ptr = std::unique_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Ptr<T> make_ptr(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}

#endif // BASE_HPP
