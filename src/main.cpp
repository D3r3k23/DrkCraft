#include "Core/Base.hpp"
#include "Core/BuildSettings.hpp"
#include "Core/RunSettings.hpp"
#include "Core/Profiler.hpp"
#include "Application/Application.hpp"
#include "Application/MainMenu.hpp"

#include <thread>

using namespace DrkCraft;

int main(int argc, char* argv[])
{
    DRK_PROFILER_BEGIN("DrkCraft", "data/profile/results.json");
    DRK_LOGGER_INIT("data/logs");

    DRK_LOG_CORE_INFO("DrkCraft Build:");
    DRK_LOG_CORE_INFO("Version: v{}", BUILD_VERSION.string());
    DRK_LOG_CORE_INFO("Platform: {}", DRK_PLATFORM_NAME);
    DRK_LOG_CORE_INFO("Config: {}",   DRK_CONFIG_NAME);

    if (DRK_PROFILER_ACTIVE())
        DRK_LOG_CORE_INFO("Profiler running");

    if constexpr (DRK_TRACE_LOGGING_ENABLED)
        DRK_LOG_CORE_INFO("Trace logging enabled");

    DRK_LOG_CORE_INFO("Loading settings");
    CommandLineOptions::parse_args(argc, argv);
    RuntimeSettings::load("config");
    DRK_LOG_CORE_INFO("Game mode: {}", game_mode_to_string(CommandLineOptions::get_game_mode()));

    DRK_LOG_CORE_INFO("{} threads supported by hardware", std::thread::hardware_concurrency());

    DRK_LOG_CORE_TRACE("Initializing Application");
    Application::init();

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
