#ifndef DRK_CORE_DEBUG_ASSERT_HPP
#define DRK_CORE_DEBUG_ASSERT_HPP

// Don't include this directly, use Core/Base.hpp instead

#include "Core/Build.hpp"

#if DRK_ASSERTS_ENABLED

    #include "Core/Type.hpp"

    #include <fmt/format.h>

    #include <string_view>
    #include <source_location>

    namespace DrkCraft
    {
        void on_assert_failure(std::string_view cond, const std::source_location& src, std::string_view msg="");
    }

    #if defined(DRK_CONFIG_DEBUG)
    #   if defined(DRK_PLATFORM_WINDOWS)
    #      define DRK_DEBUG_BREAK() __debugbreak()
    #   elif defined(DRK_PLATFORM_LINUX)
    #      include <signal.h>
    #      define DRK_DEBUG_BREAK() raise(SIGTRAP)
    #   else
    #      error "Unsupported platform"
    #   endif
    #else
    #   define DRK_DEBUG_BREAK()
    #endif

    #define DRK_ASSERT_IMPL(cond, ...)                                \
        do { if (!(cond)) {                                           \
            on_assert_failure(#cond, std::source_location::current(), \
              fmt::format(__VA_ARGS__));                              \
            DRK_DEBUG_BREAK();                                        \
        } } while (false)

    #define DRK_ASSERT_IMPL_NO_MSG(cond)                               \
        do { if (!(cond)) {                                            \
            on_assert_failure(#cond, std::source_location::current()); \
            DRK_DEBUG_BREAK();                                         \
        } } while (false)

    #define DRK_ASSERT_CORE(cond, ...)   DRK_ASSERT_IMPL(cond, __VA_ARGS__)
    #define DRK_ASSERT_CORE_NO_MSG(cond) DRK_ASSERT_IMPL_NO_MSG(cond)

    #if defined(DRK_CONFIG_DEBUG)
    #   define DRK_ASSERT_DEBUG(cond, ...)   DRK_ASSERT_IMPL(cond, __VA_ARGS__)
    #   define DRK_ASSERT_DEBUG_NO_MSG(cond) DRK_ASSERT_IMPL_NO_MSG(cond)
    #else
    #   define DRK_ASSERT_DEBUG(cond, ...)
    #   define DRK_ASSERT_DEBUG_NO_MSG(cond)
    #endif

#else
    #define DRK_ASSERT_CORE(cond, ...)
    #define DRK_ASSERT_DEBUG(cond, ...)
    #define DRK_ASSERT_CORE_NO_MSG(cond)
    #define DRK_ASSERT_DEBUG_NO_MSG(cond)
#endif

#endif // DRK_CORE_DEBUG_ASSERT_HPP
