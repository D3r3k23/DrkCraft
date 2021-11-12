#ifndef DRK_BASE_HPP
#define DRK_BASE_HPP

#include "BuildSettings.hpp"
#include "RunSettings.hpp"
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

    // Common macros
    #define DRK_EXPAND_MACRO(x) x
    #define DRK_STRINGIFY(x) #x
    #define DRK_CONCAT(a, b) a ## b
    #define DRK_VA_ARGS(...) ,##__VA_ARGS__

    // Bind member function to lambda
    #define DRK_BIND_FN(fn) [this](auto&& ... args) -> auto \
    {                                                         \
        return this->fn(std::forward<decltype(args)>(args)...); \
    }

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
