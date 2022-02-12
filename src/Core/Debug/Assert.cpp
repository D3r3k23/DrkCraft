#include "Assert.hpp"

#if DRK_ASSERTS_ENABLED

    #include "Core/Debug/Log.hpp"
    #include "Core/Debug/Profiler.hpp"

    #include "lib/fs.hpp"

    namespace DrkCraft
    {
        void on_assert_failure(std::string_view cond, const std::source_location& src, std::string_view msg)
        {
            const auto filename = fs::path(src.file_name()).filename().string();
            DRK_LOG_CORE_CRITICAL("[{0}:{1}] Assert ({2}) failed{3}{4}",
                filename, src.line(), cond, msg.length() > 0 ? ": " : "", msg);

            DRK_LOGGER_CLOSE();
            DRK_PROFILE_EVENT("Assert failure");
            DRK_PROFILER_END();
        }
    }

#endif
