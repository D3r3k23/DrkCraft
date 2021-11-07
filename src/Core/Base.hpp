#ifndef DRK_BASE_HPP
#define DRK_BASE_HPP

#include "Build.hpp"
#include "Log.hpp"

#include <memory>
#include <cstdint>

#if defined(DRK_DEBUG)
    #if defined(DRK_PLATFORM_WINDOWS)
        #define DRK_DEBUG_BREAK NOT_IMPLEMENTED
    #elif defined(DRK_PLATFORM_LINUX)
        #define DRK_DEBUG_BREAK NOT_IMPLEMENTED
    #endif
#endif

namespace DrkCraft
{
    using uint   = unsigned int;
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
