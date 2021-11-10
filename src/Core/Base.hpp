#ifndef DRK_BASE_HPP
#define DRK_BASE_HPP

#include "BuildSettings.hpp"
#include "Log.hpp"
#include "Assert.hpp"

#include <cstdint>
#include <memory>
#include <utility>

namespace DrkCraft
{
    // Alias for integral types
    using uint   = unsigned int;
    using int16  = int16_t;
    using uint16 = uint16_t;
    using int32  = int32_t;
    using uint32 = uint32_t;

    // Alias for lifetime-managed pointer
    template <typename T>
    using Ptr = std::unique_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Ptr<T> make_ptr(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    // Alias for reference-counted pointer
    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Ref<T> make_ref(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#endif // DRK_BASE_HPP
