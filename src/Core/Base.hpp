#ifndef DRK_BASE_HPP
#define DRK_BASE_HPP

#include "BuildConfig.hpp"
#include "Log.hpp"
#include "Assert.hpp"

#include <memory>
#include <cstdint>

namespace DrkCraft
{
    using uint   = unsigned int;
    using int16  = int16_t;
    using uint16 = uint16_t;
    using int32  = int32_t;
    using uint32 = uint32_t;

    template <typename T>
    using Ptr = std::unique_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Ptr<T> make_ptr(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}

#endif // DRK_BASE_HPP
