#ifndef DRK_ASSERT_HPP
#define DRK_ASSERT_HPP

// Don't include this directly, use Core/Base.hpp instead

#include "BuildSettings.hpp"

#if defined(DRK_EN_ASSERTS)

    #include <string_view>
    #include <format>
    #include <source_location>

    namespace DrkCraft
    {
        void assert_failure(std::string_view cond, const std::source_location& src, std::string_view msg);
        void assert_failure(std::string_view cond, const std::source_location& src);
    }

    #define DRK_ASSERT_IMPL(cond, ...)                              \
        do {                                                         \
            if (!(cond))                                              \
                assert_failure(#cond, std::source_location::current(), \
                    std::format(__VA_ARGS__));                          \
        } while (false)

    #define DRK_ASSERT_IMPL_NO_MSG(cond)                             \
        do {                                                          \
            if (!(cond))                                               \
                assert_failure(#cond, std::source_location::current()); \
        } while (false)

    #define DRK_ASSERT_CORE(cond, ...)   DRK_ASSERT_IMPL(cond, __VA_ARGS__)
    #define ARK_ASSERT_CORE_NO_MSG(cond) DRK_ASSERT_IMPL_NO_MSG(cond)

    #if defined(DRK_CONFIG_DEBUG)

        #define DRK_ASSERT_DEBUG(cond, ...)   DRK_ASSERT_IMPL(cond, __VA_ARGS__)
        #define DRK_ASSERT_DEBUG_NO_MSG(cond) DRK_ASSERT_IMPL_NO_MSG(cond)

    #else
        #define DRK_ASSERT_DEBUG(cond, ...)
        #define DRK_ASSERT_DEBUG_NO_MSG(cond)
    #endif

#else
    #define DRK_ASSERT_CORE(cond, ...)
    #define DRK_ASSERT_CORE_NO_MSG(cond)
    #define DRK_ASSERT_DEBUG(cond, ...)
    #define DRK_ASSERT_DEBUG_NO_MSG(cond)
#endif

#endif // DRK_ASSERT_HPP
