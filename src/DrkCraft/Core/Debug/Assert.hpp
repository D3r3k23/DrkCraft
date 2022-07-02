#ifndef DRK_CORE_DEBUG_ASSERT_HPP
#define DRK_CORE_DEBUG_ASSERT_HPP

#include "Core/Build.hpp"
#include "Core/Type.hpp"

#if DRK_ASSERTS_ENABLED
    #include "Lib/string_view.hpp"

    #include <fmt/format.h>

    #include <source_location>

    namespace DrkCraft
    {
        void on_assert_failure(const std::source_location& src, Optional<string_view> cond, Optional<string_view> msg);
    }

    #if DRK_DEBUG_ENABLED
    #   if DRK_WINDOWS_ENABLED
    #      define DRK_DEBUG_BREAK() __debugbreak()
    #   elif DRK_LINUX_ENABLED
    #      include <signal.h>
    #      define DRK_DEBUG_BREAK() raise(SIGTRAP)
    #   else
    #      error "Unsupported platform"
    #   endif
    #else
    #   define DRK_DEBUG_BREAK()
    #endif

    #define DRK_ASSERT_FAILURE(cond, ...)                       \
        do {                                                    \
            on_assert_failure(std::source_location::current(),  \
                cond, fmt::format(__VA_ARGS__));                \
            DRK_DEBUG_BREAK();                                  \
        } while (false)

    #define DRK_ASSERT_FAILURE_NO_MSG(cond)                    \
        do {                                                   \
            on_assert_failure(std::source_location::current(), \
                cond, {});                                     \
            DRK_DEBUG_BREAK();                                 \
        } while (false)

    #define DRK_ASSERT_IMPL(cond, ...)                  \
        do {                                            \
            if (!(cond)) {                              \
                DRK_ASSERT_FAILURE(#cond, __VA_ARGS__); \
            }                                           \
        } while (false)

    #define DRK_ASSERT_IMPL_NO_MSG(cond)          \
        do {                                      \
            if (!(cond)) {                        \
                DRK_ASSERT_FAILURE_NO_MSG(#cond); \
            }                                     \
        } while (false)

    #define DRK_ASSERT_CORE(cond, ...)  DRK_ASSERT_IMPL(cond, __VA_ARGS__)
    #define DRK_ASSERT_CORE_NO_MSG(cond) DRK_ASSERT_IMPL_NO_MSG(cond)
    #define DRK_ASSERT_CORE_FALSE(...)    DRK_ASSERT_FAILURE({}, __VA_ARGS__)
    #define DRK_ASSERT_CORE_FALSE_NO_MSG() DRK_ASSERT_FAILURE_NO_MSG({})

    #if DRK_DEBUG_ENABLED
    #   define DRK_ASSERT_DEBUG(cond, ...)  DRK_ASSERT_IMPL(cond, __VA_ARGS__)
    #   define DRK_ASSERT_DEBUG_NO_MSG(cond) DRK_ASSERT_IMPL_NO_MSG(cond)
    #   define DRK_ASSERT_DEBUG_FALSE(...)    DRK_ASSERT_FAILURE({}, __VA_ARGS__)
    #   define DRK_ASSERT_DEBUG_FALSE_NO_MSG() DRK_ASSERT_FAILURE_NO_MSG({})
    #else
    #   define DRK_ASSERT_DEBUG(cond, ...)
    #   define DRK_ASSERT_DEBUG_NO_MSG(cond)
    #   define DRK_ASSERT_DEBUG_FALSE(...)
    #   define DRK_ASSERT_DEBUG_FALSE_NO_MSG()
    #endif

#else
    #define DRK_ASSERT_CORE(cond, ...)
    #define DRK_ASSERT_DEBUG(cond, ...)
    #define DRK_ASSERT_CORE_FALSE(...)
    #define DRK_ASSERT_DEBUG_FALSE(...)
    #define DRK_ASSERT_CORE_NO_MSG(cond)
    #define DRK_ASSERT_DEBUG_NO_MSG(cond)
    #define DRK_ASSERT_CORE_FALSE_NO_MSG()
    #define DRK_ASSERT_DEBUG_FALSE_NO_MSG()
#endif

#endif // DRK_CORE_DEBUG_ASSERT_HPP
