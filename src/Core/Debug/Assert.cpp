#include "Assert.hpp"

#if DRK_ASSERTS_ENABLED

    #include "Core/Debug/Log.hpp"
    #include "Core/Debug/Stacktrace.hpp"
    #include "Core/Debug/Profiler.hpp"

    #include "lib/fs.hpp"

    #include <string>

    namespace DrkCraft
    {
        void on_assert_failure(const std::source_location& src, Optional<std::string_view> cond, Optional<std::string_view> msg)
        {
            DRK_PROFILE_EVENT_LOCAL("Assert failure");

            const auto filename = fs::path{src.file_name()}.filename().string();
            const auto line = src.line();

            const std::string condition = cond ? fmt::format("({}) ", *cond) : "";
            const std::string message   = msg  ? fmt::format(": {}",  *msg)  : "";
            DRK_LOG_CORE_CRITICAL("[{}:{}] Assert {}failed{}", filename, line, condition, message);

        #if DRK_STACKTRACE_ENABLED
            DRK_LOGGER_FLUSH();
            DRK_PRINT_STACKTRACE();
        #endif

            DRK_LOGGER_CLOSE();
            DRK_PROFILER_END();
        }
    }

#endif
