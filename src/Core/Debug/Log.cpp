#include "Log.hpp"

#if DRK_LOGGING_ENABLED

    #include "Util/Time.hpp"
    #include "Core/Settings.hpp"
    #include "Core/Debug/Profiler.hpp"

    #include "lib/fs.hpp"

    #include <spdlog/spdlog.h>
    #include <spdlog/sinks/stdout_color_sinks.h>
    #include <spdlog/sinks/basic_file_sink.h>
    #include <spdlog/sinks/dist_sink.h>

    #include <fmt/format.h>
    #include <fmt/chrono.h>

    #include <algorithm>

    namespace DrkCraft
    {
        const char* Logger::s_name = nullptr;

        Ref<spdlog::logger> Logger::s_coreLogger;
        Ref<spdlog::logger> Logger::s_gameLogger;
        Ref<spdlog::logger> Logger::s_eventLogger;

        using DistSink = spdlog::sinks::dist_sink_mt;
        using LogLevel = spdlog::level::level_enum;

        static Ref<DistSink> create_sink(const fs::path& logFile, LogLevel fileLevel, LogLevel consoleLevel=LogLevel::off);

        void Logger::init(const char* logName, const char* dir)
        {
            DRK_PROFILE_FUNCTION();

            s_name = logName;

            const auto time = Time::get_system_time();
            const auto name = fmt::format("{}_{:%Y-%m-%d_%H.%M.%S}.log", logName, fmt::localtime(time));
            const auto file = fs::path(dir) / fs::path(name);

            LogLevel fileLevel;
            LogLevel consoleLevel;
            if constexpr (DRK_DEBUG_ENABLED)
            {
                fileLevel = LogLevel::trace;
                if (CommandLineOptions::get_options().en_trace_log)
                    consoleLevel = LogLevel::trace;
                else
                    consoleLevel = LogLevel::debug;
            }
            else
            {
                fileLevel = LogLevel::info;
                consoleLevel = LogLevel::off;
            }
            LogLevel staticLevel = std::min(fileLevel, consoleLevel);

            auto sink = create_sink(file, fileLevel, consoleLevel);
            sink->set_level(staticLevel);

            s_coreLogger = make_ref<spdlog::logger>("Core", sink);
            s_coreLogger->flush_on(spdlog::level::err);
            s_coreLogger->set_level(staticLevel);

            s_gameLogger = make_ref<spdlog::logger>("Game", sink);
            s_gameLogger->flush_on(spdlog::level::err);
            s_gameLogger->set_level(staticLevel);

            s_eventLogger = make_ref<spdlog::logger>("Event", sink);
            s_eventLogger->flush_on(spdlog::level::err);
            s_eventLogger->set_level(staticLevel);

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

        Ref<DistSink> create_sink(const fs::path& logFile, LogLevel fileLevel, LogLevel consoleLevel)
        {
            auto sink = make_ref<spdlog::sinks::dist_sink_mt>();
            if (consoleLevel < LogLevel::off)
            {
                auto consoleSink = make_ref<spdlog::sinks::stdout_color_sink_mt>();
                consoleSink->set_level(consoleLevel);
                sink->add_sink(consoleSink);
            }
            auto fileSink = make_ref<spdlog::sinks::basic_file_sink_mt>(logFile.string());
            fileSink->set_level(fileLevel);
            sink->add_sink(fileSink);

            sink->set_pattern("[%Y-%m-%d %T.%e] [%^%n:%l%$] %v");

            return sink;
        }
    }

#endif
