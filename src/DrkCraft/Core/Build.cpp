#include "Build.hpp"

#include "Core/Debug/Log.hpp"
#include "System/Thread.hpp"

namespace DrkCraft::Build
{
    void log_info(void)
    {
        DRK_LOG_CORE_INFO("Version: v{}",  DRK_VERSION_STRING);
        DRK_LOG_CORE_INFO("Platform: {}", DRK_PLATFORM_NAME);
        DRK_LOG_CORE_INFO("Config: {}",  DRK_CONFIG_NAME);

        if constexpr (DRK_PROFILING_ENABLED)
            DRK_LOG_CORE_INFO("Profiler enabled");

        DRK_LOG_CORE_DEBUG("{} threads supported by hardware", Thread<>::count());
    }
}
