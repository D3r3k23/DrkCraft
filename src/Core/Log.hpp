#ifndef DRK_LOG_HPP
#define DRK_LOG_HPP

// Don't include this directly, use Core/Base.hpp instead

#include "BuildSettings.hpp"

#if defined(DRK_EN_LOGGING)

    #include <spdlog/spdlog.h>

    #include <string>
    #include <string_view>

    namespace DrkCraft
    {
        class Event;

        class Logger
        {
        public:
            static void init(std::string_view name);
            static void close(void);

            static void log_event(const Event& event);

        private:
            static std::string s_name;

            // s_consoleLogger
            // s_fileLogger

            // s_coreLog
            // s_eventLog
            // s_rendererLog
            // s_gameLog
        };
    }

    // log_event
    // glm types log helpers - just #include <glm/gtx/string_cast.hpp>: glm::to_string(x)

    #define DRK_LOGGER_INIT(name) Logger::init(name)
    #define DRK_LOGGER_CLOSE()    Logger::close()

    #define DRK_LOG_TRACE(...)    spdlog::trace(__VA_ARGS__)
    #define DRK_LOG_INFO(...)     spdlog::info(__VA_ARGS__)
    #define DRK_LOG_WARN(...)     spdlog::warn(__VA_ARGS__)
    #define DRK_LOG_ERROR(...)    spdlog::error(__VA_ARGS__)
    #define DRK_LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)

    #define DRK_LOG_EVENT(event) Logger::log_event(event)

#else
    #define DRK_LOGGER_INIT()
    #define DRK_LOGGER_CLOSE()

    #define DRK_LOG_TRACE(...)
    #define DRK_LOG_INFO(...)
    #define DRK_LOG_WARN(...)
    #define DRK_LOG_ERROR(...)
    #define DRK_LOG_CRITICAL(...)

    #define DRK_LOG_EVENT(event)
#endif

#endif // DRK_LOG_HPP
