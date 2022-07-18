#include "Core/Base.hpp"
#include "Core/Settings.hpp"
#include "Core/Debug/Profiler.hpp"
#include "Application/Application.hpp"
#include "Application/Layers/MainMenu.hpp"

#include <Argv/Argv.hpp>

namespace DrkCraft
{
    int Main(Argv::Argv argv)
    {
        const auto& options = CommandLineOptions::parse_args(argv);

        DRK_PROFILER_BEGIN("DrkCraft", "data/profile/results.json");
        DRK_LOGGER_INIT("DrkCraft", "data/logs", options.en_trace_log);

        DRK_LOG_CORE_INFO("DrkCraft build info:");
        Build::log_info();

        DRK_LOG_CORE_INFO("DrkCraft command line options:");
        CommandLineOptions::log_options();

        DRK_LOG_CORE_INFO("Loading settings");
        RuntimeSettings::load("config");

        DRK_LOG_CORE_INFO("Initializing Application");
        Application::init("DrkCraft");

        DRK_LOG_CORE_INFO("Opening Main Menu");
        Application::add_layer(Layer::create<MainMenu>());

        DRK_PROFILE_EVENT_GLOBAL("runtime_start");
        {
            DRK_LOG_CORE_INFO("Running Application");
            Application::run();
        }
        DRK_PROFILE_EVENT_GLOBAL("runtime_end");

        DRK_LOG_CORE_INFO("Shutting down Application");
        int status = Application::shutdown();

        DRK_LOGGER_CLOSE();
        DRK_PROFILER_END();

        return status;
    }
}
