#include "Log.hpp"

#if DRK_LOGGING_ENABLED

    #include "Util/Time.hpp"
    #include "Core/Debug/Profiler.hpp"

    #include "lib/fs.hpp"

    #include <spdlog/spdlog.h>
    #include <spdlog/sinks/stdout_color_sinks.h>
    #include <spdlog/sinks/basic_file_sink.h>
    #include <spdlog/sinks/dist_sink.h>

    #include <fmt/format.h>
    #include <fmt/chrono.h>

    namespace DrkCraft
    {
        const char* Logger::s_name = nullptr;

        Ref<spdlog::logger> Logger::s_coreLogger;
        Ref<spdlog::logger> Logger::s_gameLogger;
        Ref<spdlog::logger> Logger::s_eventLogger;

        using LogLevel = spdlog::level::level_enum;

    #if DRK_DEBUG_ENABLED
        constexpr LogLevel STATIC_LOG_LEVEL  = LogLevel::trace;
        constexpr LogLevel CONSOLE_LOG_LEVEL = DRK_TRACE_LOGGING_ENABLED ? LogLevel::trace : LogLevel::debug;
        constexpr bool CONSOLE_LOG_ENABLED = true;
    #else
        constexpr LogLevel STATIC_LOG_LEVEL  = LogLevel::info;
        constexpr LogLevel CONSOLE_LOG_LEVEL = LogLevel::off;
        constexpr bool CONSOLE_LOG_ENABLED = false;
    #endif
        constexpr LogLevel FILE_LOG_LEVEL = STATIC_LOG_LEVEL;

        void Logger::init(const char* logName, const char* dir)
        {
            DRK_PROFILE_FUNCTION();

            s_name = logName;

            auto time = Time::get_system_time();
            auto name = fmt::format("{}_{:%Y-%m-%d_%H.%M.%S}.log", logName, fmt::localtime(time));
            auto file = fs::path(dir) / fs::path(name);

            auto sink = make_ref<spdlog::sinks::dist_sink_mt>();
            if constexpr (CONSOLE_LOG_ENABLED)
            {
                auto consoleSink = make_ref<spdlog::sinks::stdout_color_sink_mt>();
                consoleSink->set_level(CONSOLE_LOG_LEVEL);
                sink->add_sink(consoleSink);
            }
            auto fileSink = make_ref<spdlog::sinks::basic_file_sink_mt>(file.string());
            fileSink->set_level(FILE_LOG_LEVEL);
            sink->add_sink(fileSink);

            sink->set_pattern("[%Y-%m-%d %T.%e] [%^%n:%l%$] %v");
            sink->set_level(STATIC_LOG_LEVEL);

            s_coreLogger = make_ref<spdlog::logger>("Core", sink);
            s_coreLogger->flush_on(spdlog::level::err);
            s_coreLogger->set_level(STATIC_LOG_LEVEL);

            s_gameLogger = make_ref<spdlog::logger>("Game", sink);
            s_gameLogger->flush_on(spdlog::level::err);
            s_gameLogger->set_level(STATIC_LOG_LEVEL);

            s_eventLogger = make_ref<spdlog::logger>("Event", sink);
            s_eventLogger->flush_on(spdlog::level::err);
            s_eventLogger->set_level(STATIC_LOG_LEVEL);

            DRK_LOG_CORE_INFO("{} Logger initialized", s_name);
        }

        void Logger::close(void)
        {
            DRK_PROFILE_FUNCTION();
            DRK_LOG_CORE_INFO("Closing {} Logger", s_name);

            flush();
        }

        void Logger::flush(void)
        {
            DRK_PROFILE_FUNCTION();
            DRK_LOG_CORE_TRACE("Flushing Logger");

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
