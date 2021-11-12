#include "Assert.hpp"

#if defined(DRK_EN_ASSERTS)

    #include "Log.hpp"
    #include "Profiler.hpp"

    #define DRK_ASSERT_ABORT() \
        DRK_PROFILER_END();  \
        DRK_LOGGER_CLOSE(); \
        DRK_DEBUG_BREAK();  \

    namespace DrkCraft
    {
        void assert_failure(std::string_view cond, const std::source_location& src, std::string_view msg)
        {
            DRK_LOG_CRITICAL("[{0}:{1}] Assert ({2}) failed: {3}", src.file_name(), src.line(), cond, msg);
            DRK_ASSERT_ABORT();
        }

        void assert_failure(std::string_view cond, const std::source_location& src)
        {
            DRK_LOG_CRITICAL("[{0}:{1}] Assert ({2}) failed", src.file_name(), src.line(), cond);
            DRK_ASSERT_ABORT();
        }
    }

#endif
