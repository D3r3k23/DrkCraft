#include "Log.hpp"

#if defined(DRK_EN_LOGGING)

    #include "Time.hpp"
    #include "Application/Events.hpp"

    #include <spdlog/sinks/stdout_color_sinks.h>
    #include <spdlog/sinks/basic_file_sink.h>
    #include <spdlog/sinks/dist_sink.h>

    #include <vector>
    #include <string>
    #include <string_view>
    #include <format>

    namespace DrkCraft
    {
        std::shared_ptr<spdlog::logger> Logger::s_coreLogger;
        std::shared_ptr<spdlog::logger> Logger::s_gameLogger;

        using std::filesystem::path;

        void Logger::init(path dir)
        {
            auto time = Time::get_local_time();
            auto logName = std::format("DrkCraft_{:%F_%H.%M.%S}.log", time);
            auto fileName = (dir / path(logName)).string();

            auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
            consoleSink->set_level(spdlog::level::debug);

            auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_st>(fileName);
            fileSink->set_level(spdlog::level::trace);

            auto sink = std::make_shared<spdlog::sinks::dist_sink_st>();
            sink->set_sinks({ consoleSink, fileSink });
            sink->set_pattern("[%Y-%m-%d %T.%e] [%^%n:%l%$] %v");
            sink->set_level(spdlog::level::DRK_STATIC_LOG_LEVEL);

            s_coreLogger = std::make_shared<spdlog::logger>("Core", sink);
            s_coreLogger->flush_on(spdlog::level::err);
            s_coreLogger->set_level(spdlog::level::DRK_STATIC_LOG_LEVEL);

            s_gameLogger = std::make_shared<spdlog::logger>("Game", sink);
            s_gameLogger->flush_on(spdlog::level::err);
            s_gameLogger->set_level(spdlog::level::trace);

            DRK_LOG_CORE_INFO("DrkCraft Logger initialized");
        }

        void Logger::close(void)
        {
            DRK_LOG_CORE_INFO("Closing DrkCraft Logger");
            s_coreLogger->flush();
            s_gameLogger->flush();
        }

        std::shared_ptr<spdlog::logger>& Logger::get_core_logger(void)
        {
            return s_coreLogger;
        }

        std::shared_ptr<spdlog::logger>& Logger::get_game_logger(void)
        {
            return s_gameLogger;
        }

        void Logger::log_event(const Event& event)
        {
            switch (event.get_type())
            {
                case EventType::WindowResized      : break;
                case EventType::FramebufferResized : break;
                case EventType::WindowRefreshed    : break;
                case EventType::WindowMoved        : break;
                case EventType::MouseMoved         : break;
                case EventType::CharTyped          : break;
                case EventType::KeyHeld            : break;
                default:
                    DRK_LOG_CORE_INFO("[{0}:{1}handled] {2}", event.get_name(),
                        !event.handled() ? "un" : "", event.get_details());
            }
        }
    }

#endif
