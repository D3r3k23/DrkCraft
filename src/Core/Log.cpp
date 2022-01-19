#include "Log.hpp"

#if DRK_LOGGING_ENABLED

    #include "Time.hpp"
    #include "Application/Events.hpp"

    #include <spdlog/sinks/stdout_color_sinks.h>
    #include <spdlog/sinks/basic_file_sink.h>
    #include <spdlog/sinks/dist_sink.h>

    #include <fmt/format.h>
    #include <fmt/chrono.h>

    #include <filesystem>

    namespace DrkCraft
    {
        std::shared_ptr<spdlog::logger> Logger::s_coreLogger;
        std::shared_ptr<spdlog::logger> Logger::s_gameLogger;
        std::shared_ptr<spdlog::logger> Logger::s_eventLogger;

        using LogLevel = spdlog::level::level_enum;

    #if defined(DRK_CONFIG_DEBUG)
        constexpr LogLevel STATIC_LOG_LEVEL  = LogLevel::trace;
        constexpr LogLevel CONSOLE_LOG_LEVEL = DRK_TRACE_LOGGING_ENABLED ?
                                               LogLevel::trace : LogLevel::debug;
        constexpr bool CONSOLE_LOG_ENABLED = true;
    #else
        constexpr LogLevel STATIC_LOG_LEVEL  = LogLevel::info;
        constexpr LogLevel CONSOLE_LOG_LEVEL = LogLevel::off;
        constexpr bool CONSOLE_LOG_ENABLED = false;
    #endif
        constexpr LogLevel FILE_LOG_LEVEL = STATIC_LOG_LEVEL;

        void Logger::init(const char* dir)
        {
            using std::filesystem::path;

            auto time  =  Time::get_system_time();
            auto logName = fmt::format("DrkCraft_{:%Y-%m-%d_%H.%M.%S}.log", fmt::localtime(time));
            auto file  =  path(dir) / path(logName);

            auto sink = std::make_shared<spdlog::sinks::dist_sink_mt>();
            if constexpr (CONSOLE_LOG_ENABLED)
            {
                auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                consoleSink->set_level(CONSOLE_LOG_LEVEL);
                sink->add_sink(consoleSink);
            }{
                auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(file.string());
                fileSink->set_level(FILE_LOG_LEVEL);
                sink->add_sink(fileSink);
            }
            sink->set_pattern("[%Y-%m-%d %T.%e] [%^%n:%l%$] %v");
            sink->set_level(STATIC_LOG_LEVEL);

            s_coreLogger = std::make_shared<spdlog::logger>("Core", sink);
            s_coreLogger->flush_on(spdlog::level::err);
            s_coreLogger->set_level(STATIC_LOG_LEVEL);

            s_gameLogger = std::make_shared<spdlog::logger>("Game", sink);
            s_gameLogger->flush_on(spdlog::level::err);
            s_gameLogger->set_level(STATIC_LOG_LEVEL);

            s_eventLogger = std::make_shared<spdlog::logger>("Event", sink);
            s_eventLogger->flush_on(spdlog::level::err);
            s_eventLogger->set_level(STATIC_LOG_LEVEL);

            DRK_LOG_CORE_INFO("DrkCraft Logger initialized");
        }

        void Logger::close(void)
        {
            DRK_LOG_CORE_INFO("Closing DrkCraft Logger");
            s_coreLogger->flush();
            s_gameLogger->flush();
            s_eventLogger->flush();
        }

        spdlog::logger& Logger::get_core_logger(void)
        {
            return *s_coreLogger;
        }

        spdlog::logger& Logger::get_game_logger(void)
        {
            return *s_gameLogger;
        }

        spdlog::logger& Logger::get_event_logger(void)
        {
            return *s_eventLogger;
        }
    }

#endif
