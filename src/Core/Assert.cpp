#include "Assert.hpp"

#if DRK_ASSERTS_ENABLED

    #include "Log.hpp"
    #include "Profiler.hpp"

    #include <filesystem>

    namespace DrkCraft
    {
        using std::filesystem::path;

        void on_assert_failure(std::string_view cond, const std::source_location& src, std::string_view msg)
        {
            auto filename = path(src.file_name()).filename().string();
            DRK_LOG_CORE_CRITICAL("[{0}:{1}] Assert ({2}) failed: {3}", filename, src.line(), cond, msg);
            DRK_LOGGER_CLOSE();
            DRK_PROFILE_EVENT("Assert failure");
            DRK_PROFILER_END();
        }
    }

#endif
