#include "Core/Base.hpp"
#include "Core/Settings.hpp"
#include "Core/Debug/Profiler.hpp"
#include "Application/Application.hpp"
#include "Application/Layers/MainMenu.hpp"

#include <thread>

using namespace DrkCraft;

int main(int argc, char* argv[])
{
    CommandLineOptions::parse_args(argc, argv);

    DRK_PROFILER_BEGIN("DrkCraft", "data/profile/results.json");
    DRK_LOGGER_INIT("DrkCraft", "data/logs");

    DRK_LOG_CORE_INFO("DrkCraft Build:");
    DRK_LOG_CORE_INFO("Version: v{}",  DRK_VERSION_STRING);
    DRK_LOG_CORE_INFO("Platform: {}", DRK_PLATFORM_NAME);
    DRK_LOG_CORE_INFO("Config: {}",  DRK_CONFIG_NAME);

    if constexpr (DRK_PROFILING_ENABLED) DRK_LOG_CORE_INFO("Profiler enabled");

    if (CommandLineOptions::get_options().en_dev_mode)  DRK_LOG_CORE_INFO("Dev mode enabled");
    if (CommandLineOptions::get_options().en_trace_log) DRK_LOG_CORE_INFO("Trace logging enabled");

    DRK_LOG_CORE_INFO("{} threads supported by hardware", std::thread::hardware_concurrency());

    DRK_LOG_CORE_INFO("Loading settings");
    RuntimeSettings::load("config");

    DRK_LOG_CORE_TRACE("Initializing Application");
    Application::init("DrkCraft");

    DRK_LOG_CORE_TRACE("Opening Main Menu");
    Application::add_layer(Layer::create<MainMenu>());

    DRK_LOG_CORE_TRACE("Running Application");
    Application::run();

    DRK_LOG_CORE_TRACE("Shutting down Application");
    int status = Application::shutdown();

    DRK_LOGGER_CLOSE();
    DRK_PROFILER_END();

    return status;
}
