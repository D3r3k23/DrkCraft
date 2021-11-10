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
        class Logger
        {
        public:
            static void init(std::string_view name);
            static void save(void);

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

    #define DRK_LOGGER_INIT(name) DrkCraft::Logger::init(name)
    #define DRK_LOGGER_SAVE()     DrkCraft::Logger::save() // What does this do

    #define DRK_LOG_TRACE(...)    spdlog::trace(__VA_ARGS__)
    #define DRK_LOG_INFO(...)     spdlog::info(__VA_ARGS__)
    #define DRK_LOG_WARN(...)     spdlog::warn(__VA_ARGS__)
    #define DRK_LOG_ERROR(...)    spdlog::error(__VA_ARGS__)
    #define DRK_LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)

#else
    #define DRK_LOGGER_INIT(name)
    #define DRK_LOGGER_SAVE()

    #define DRK_LOG_TRACE(...)
    #define DRK_LOG_INFO(...)
    #define DRK_LOG_WARN(...)
    #define DRK_LOG_ERROR(...)
    #define DRK_LOG_CRITICAL(...)
#endif

#endif // DRK_LOG_HPP
