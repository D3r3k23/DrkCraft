#include "Assert.hpp"

#if defined(DRK_EN_ASSERTS)

    #include "Log.hpp"

    #include "Profiler.hpp"

    namespace DrkCraft
    {
        void assert_failed(const char* cond, const char* msg, const std::source_location& src)
        {
            DRK_LOG_CRITICAL("[{0}:{1}] Assert ({2}) failed: {3}", src.file_name(), src.line(), cond, msg);
            DRK_PROFILER_END();
            DRK_LOGGER_CLOSE();
        }
    }

#endif
