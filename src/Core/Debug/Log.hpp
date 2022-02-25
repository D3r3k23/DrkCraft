#ifndef DRK_CORE_DEBUG_LOG_HPP
#define DRK_CORE_DEBUG_LOG_HPP

// Don't include this directly, use Core/Base.hpp instead

#include "Core/Build.hpp"

#if DRK_LOGGING_ENABLED

    #include "Core/Type.hpp"
    #include "Core/SmartPointer.hpp"

    #include <spdlog/logger.h>

    namespace DrkCraft
    {
        class Logger
        {
        public:
            static void init(const char* name, const char* dir);
            static void close(void);
            static void flush(void);

            static spdlog::logger& get_core_logger(void);
            static spdlog::logger& get_game_logger(void);
            static spdlog::logger& get_event_logger(void);

        private:
            static const char* s_name;

            static Ref<spdlog::logger> s_coreLogger;
            static Ref<spdlog::logger> s_gameLogger;
            static Ref<spdlog::logger> s_eventLogger;
        };
    }

    #define DRK_LOGGER_INIT(name, dir) Logger::init(name, dir)
    #define DRK_LOGGER_CLOSE()         Logger::close()
    #define DRK_LOGGER_FLUSH()         Logger::flush()

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

    #define DRK_LOG_EVENT_TRACE(...)
    #define DRK_LOG_EVENT_DEBUG(...)
    #define DRK_LOG_EVENT_INFO(...)
    #define DRK_LOG_EVENT_WARN(...)
    #define DRK_LOG_EVENT_ERROR(...)
    #define DRK_LOG_EVENT_CRITICAL(...)
#endif

#endif // DRK_CORE_DEBUG_LOG_HPP
