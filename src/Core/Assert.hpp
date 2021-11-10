#ifndef DRK_ASSERT_HPP
#define DRK_ASSERT_HPP

// Don't include this directly, use Core/Base.hpp instead

#include "BuildSettings.hpp"

#if defined(DRK_CONFIG_DEBUG)
    #if defined(DRK_PLATFORM_WINDOWS)
        #define DRK_DEBUG_BREAK() __debugbreak()
    #elif defined(DRK_PLATFORM_LINUX)
        #include <signal.h>
        #define DRK_DEBUG_BREAK() raise(SIGTRAP)
    #else
        #error "Unknown platform detected"
    #endif
#else
    #define DRK_DEBUG_BREAK()
#endif

#if defined(DRK_EN_ASSERTS)

    #include <string_view>

    namespace DrkCraft
    {
        // Should add formatting to msg?
        // Should assert errors be logged even if debug break is disabled?
        // Idea:
        //   DRK_ASSERT_CRIT - always enabled (in release, just aborts)
        //   DRK_ASSERT_CORE - enabled in debug
        void assert_failed(std::string_view cond, std::string_view msg, std::string_view file, int line);
    }

    #define DRK_ASSERT(cond, msg)                              \
        do {                                                   \
            if (!(cond)) {                                     \
                assert_failed(#cond, msg, __FILE__, __LINE__); \
                DRK_DEBUG_BREAK();                             \
            }                                                  \
        } while (false)

    #define DRK_ASSERT_FALSE(msg) assert_failed("false", msg, __FILE__, __LINE__)

#else
    #define DRK_ASSERT(cond, msg)
    #define DRK_ASSERT_FALSE(msg)
#endif

#endif // DRK_ASSERT_HPP
