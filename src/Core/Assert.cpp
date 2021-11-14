#include "Assert.hpp"

#if defined(DRK_EN_ASSERTS)

    #include "Log.hpp"
    #include "Profiler.hpp"

    #if defined(DRK_PLATFORM_WINDOWS)
        #define DRK_DEBUG_BREAK() __debugbreak()
    #elif defined(DRK_PLATFORM_LINUX)
        #include <signal.h>
        #define DRK_DEBUG_BREAK() raise(SIGTRAP)
    #else
        #error "Unsupported platform"
    #endif

    namespace DrkCraft
    {
        void assert_failure(std::string_view cond, const std::source_location& src, std::string_view msg)
        {
            DRK_LOG_CRITICAL("[{0}:{1}] Assert ({2}) failed: {3}", src.file_name(), src.line(), cond, msg);
            DRK_PROFILER_END();
            DRK_LOGGER_CLOSE();
            DRK_DEBUG_BREAK();
        }

        void assert_failure(std::string_view cond, const std::source_location& src)
        {
            DRK_LOG_CRITICAL("[{0}:{1}] Assert ({2}) failed", src.file_name(), src.line(), cond);
            DRK_PROFILER_END();
            DRK_LOGGER_CLOSE();
            DRK_DEBUG_BREAK();
        }
    }

#endif
