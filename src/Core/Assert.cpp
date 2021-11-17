#include "Assert.hpp"

#if defined(DRK_EN_ASSERTS)

    #include "Log.hpp"
    #include "Profiler.hpp"

    #include <filesystem>

    using std::filesystem::path;

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
            auto filename = path(src.file_name()).filename().string();
            DRK_LOG_CORE_CRITICAL("[{0}:{1}] Assert ({2}) failed: {3}", filename, src.line(), cond, msg);
            if (DRK_PROFILER_ACTIVE())
                DRK_PROFILER_END();
            DRK_LOGGER_CLOSE();
            DRK_DEBUG_BREAK();
        }

        void assert_failure(std::string_view cond, const std::source_location& src)
        {
            auto filename = path(src.file_name()).filename().string();
            DRK_LOG_CORE_CRITICAL("[{0}:{1}] Assert ({2}) failed", filename, src.line(), cond);
            if (DRK_PROFILER_ACTIVE())
                DRK_PROFILER_END();
            DRK_LOGGER_CLOSE();
            DRK_DEBUG_BREAK();
        }
    }

#endif
