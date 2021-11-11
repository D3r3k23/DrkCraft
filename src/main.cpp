#include "Core/BuildSettings.hpp"
#include "Core/Base.hpp"
#include "Application/Application.hpp"
#include "Core/Profiler.hpp"

using namespace DrkCraft;

int main(int, char**)
{
    DRK_LOGGER_INIT();

    DRK_LOG_INFO("Version: {}", DRK_VERSION_STRING);
    DRK_LOG_INFO("Build config: {}", DRK_CONFIG_NAME);
    DRK_LOG_INFO("Platform: {}", DRK_PLATFORM_NAME);
#if defined(DRK_EN_PROFILE)
    DRK_LOG_INFO("Profiling enabled");
#endif

    DRK_PROFILER_BEGIN("profile/results.json");

    Application::init();
    Application::start();
    int status = Application::shutdown();

    DRK_PROFILER_END();
    DRK_LOGGER_CLOSE();

    return status;
}
