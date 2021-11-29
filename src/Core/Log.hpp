#ifndef DRK_LOG_CORE_HPP
#define DRK_LOG_CORE_HPP

// Don't include this directly, use Core/Base.hpp instead

#include "BuildSettings.hpp"

#if defined(DRK_EN_LOGGING)

    #include <spdlog/spdlog.h>

    #include <memory>
    #include <filesystem>

    namespace DrkCraft
    {
        class Event;

        class Logger
        {
        public:
            static void init(const std::filesystem::path& dir);
            static void close(void);

            static spdlog::logger& get_core_logger(void);
            static spdlog::logger& get_game_logger(void);
            static spdlog::logger& get_event_logger(void);

            static void log_event(const Event& event);

        private:
            static std::shared_ptr<spdlog::logger> s_coreLogger;
            static std::shared_ptr<spdlog::logger> s_gameLogger;
            static std::shared_ptr<spdlog::logger> s_eventLogger;
        };
    }

    #define DRK_LOGGER_INIT(dir) Logger::init(dir)
    #define DRK_LOGGER_CLOSE()   Logger::close()

    #define DRK_LOG_CORE_TRACE(...)    Logger::get_core_logger().trace(__VA_ARGS__)
    #define DRK_LOG_CORE_DEBUG(...)    Logger::get_core_logger().debug(__VA_ARGS__)
    #define DRK_LOG_CORE_INFO(...)     Logger::get_core_logger().info(__VA_ARGS__)
    #define DRK_LOG_CORE_WARN(...)     Logger::get_core_logger().warn(__VA_ARGS__)
    #define DRK_LOG_CORE_ERROR(...)    Logger::get_core_logger().error(__VA_ARGS__)
    #define DRK_LOG_CORE_CRITICAL(...) Logger::get_core_logger().critical(__VA_ARGS__)

    #define DRK_LOG_GAME_TRACE(...)    Logger::get_game_logger().trace(__VA_ARGS__)
    #define DRK_LOG_GAME_DEBUG(...)    Logger::get_game_logger().debug(__VA_ARGS__)
    #define DRK_LOG_GAME_INFO(...)     Logger::get_game_logger().info(__VA_ARGS__)
    #define DRK_LOG_GAME_WARN(...)     Logger::get_game_logger().warn(__VA_ARGS__)
    #define DRK_LOG_GAME_ERROR(...)    Logger::get_game_logger().error(__VA_ARGS__)
    #define DRK_LOG_GAME_CRITICAL(...) Logger::get_game_logger().critical(__VA_ARGS__)

    #define DRK_LOG_EVENT_TRACE(...)    Logger::get_event_logger().trace(__VA_ARGS__)
    #define DRK_LOG_EVENT_DEBUG(...)    Logger::get_event_logger().debug(__VA_ARGS__)
    #define DRK_LOG_EVENT_INFO(...)     Logger::get_event_logger().info(__VA_ARGS__)
    #define DRK_LOG_EVENT_WARN(...)     Logger::get_event_logger().warn(__VA_ARGS__)
    #define DRK_LOG_EVENT_ERROR(...)    Logger::get_event_logger().error(__VA_ARGS__)
    #define DRK_LOG_EVENT_CRITICAL(...) Logger::get_event_logger().critical(__VA_ARGS__)

#else
    #define DRK_LOGGER_INIT(dir)
    #define DRK_LOGGER_CLOSE()

    #define DRK_LOG_CORE_TRACE(...)
    #define DRK_LOG_CORE_DEBUG(...)
    #define DRK_LOG_CORE_INFO(...)
    #define DRK_LOG_CORE_WARN(...)
    #define DRK_LOG_CORE_ERROR(...)
    #define DRK_LOG_CORE_CRITICAL(...)

    #define DRK_LOG_GAME_TRACE(...)
    #define DRK_LOG_GAME_DEBUG(...)
    #define DRK_LOG_GAME_INFO(...)
    #define DRK_LOG_GAME_WARN(...)
    #define DRK_LOG_GAME_ERROR(...)
    #define DRK_LOG_GAME_CRITICAL(...)

    #define DRK_LOG_EVENT_TRACE(event)
    #define DRK_LOG_EVENT_DEBUG(event)
    #define DRK_LOG_EVENT_INFO(event)
    #define DRK_LOG_EVENT_WARN(event)
    #define DRK_LOG_EVENT_ERROR(event)
    #define DRK_LOG_EVENT_CRITICAL(event)
#endif

#endif // DRK_LOG_CORE_HPP
