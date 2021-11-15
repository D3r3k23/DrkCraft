#include "Core/BuildSettings.hpp"
#include "Core/RunSettings.hpp"
#include "Core/Base.hpp"
#include "Application/Application.hpp"
#include "Application/MainMenu.hpp"
#include "Core/Profiler.hpp"

using namespace DrkCraft;

int main(int argc, char* argv[])
{
    DRK_LOGGER_INIT("data/logs");

    DRK_LOG_CORE_INFO("Loading runtime settings");
    RunSettings::parse_args(argc, argv);

    DRK_LOG_CORE_INFO("Version: {}", DRK_VERSION_STRING);
    DRK_LOG_CORE_INFO("Platform: {}", DRK_PLATFORM_NAME);
    DRK_LOG_CORE_INFO("Build config: {}", DRK_CONFIG_NAME);
#if defined(DRK_EN_PROFILE)
    DRK_LOG_CORE_INFO("Profiling enabled");
#endif
    DRK_LOG_CORE_INFO("Game mode: {}", RunSettings::get_game_mode_str());

    DRK_PROFILER_BEGIN("DrkCraft", "data/profile/results.json");

    DRK_LOG_CORE_TRACE("Initializing Application");
    Application::init();

    DRK_LOG_CORE_TRACE("Opening Main Menu");
    Application::get_instance().add_layer(Layer::create<MainMenu>());

    DRK_LOG_CORE_TRACE("Running Application");
    Application::get_instance().run();

    DRK_LOG_CORE_TRACE("Shutting down Application");
    int status = Application::shutdown();

    DRK_PROFILER_END();
    DRK_LOGGER_CLOSE();

    return status;
}
