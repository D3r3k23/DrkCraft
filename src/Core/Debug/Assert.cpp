#include "Assert.hpp"

#if DRK_ASSERTS_ENABLED

    #include "Core/Debug/Log.hpp"
    #include "Core/Debug/Profiler.hpp"

    #include "lib/fs.hpp"

    #include <string>

    #if DRK_DEBUG_ENABLED
    #   define DRK_STACKTRACE_ENABLED
    #endif

    #if defined(DRK_STACKTRACE_ENABLED)
        #include "backward/backward.hpp"

        #include <iostream>

        #define DRK_PRINT_STACKTRACE()  \
            StackTrace DRK_stacktrace;   \
            DRK_stacktrace.load_here(16); \
            print(DRK_stacktrace)
    #else
        #define DRK_PRINT_STACKTRACE()
    #endif

    namespace DrkCraft
    {
    #if defined(DRK_STACKTRACE_ENABLED)
        using backward::StackTrace;
        using StackTracePrinter = backward::Printer;

        static void print(const StackTrace& st)
        {
            StackTracePrinter printer;
            printer.color_mode = backward::ColorMode::automatic;
            printer.object = true;
            printer.address = true;
            printer.print(st, std::cout);
        }
    #endif

        void on_assert_failure(const std::source_location& src, Optional<std::string_view> cond, Optional<std::string_view> msg)
        {
            DRK_PROFILE_EVENT_LOCAL("Assert failure");

            const auto filename = fs::path{src.file_name()}.filename().string();
            const auto line = src.line();

            const std::string condition = cond ? fmt::format("({}) ", *cond) : "";
            const std::string message   = msg  ? fmt::format(": {}",  *msg)  : "";
            DRK_LOG_CORE_CRITICAL("[{}:{}] Assert {}failed{}", filename, line, condition, message);

            DRK_LOGGER_FLUSH();
            DRK_PRINT_STACKTRACE();

            DRK_LOGGER_CLOSE();
            DRK_PROFILER_END();
        }
    }

#endif
