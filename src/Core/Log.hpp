#ifndef DRK_LOG_HPP
#define DRK_LOG_HPP

// Don't include this directly, use Core/Base.hpp instead

#include "BuildConfig.hpp"

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
        };
    }

    #define DRK_LOGGER_INIT(name) DrkCraft::Logger::init(name)
    #define DRK_LOGGER_SAVE()     DrkCraft::Logger::save()

    #define DRK_LOG_INFO(...)  spdlog::info(__VA_ARGS__)
    #define DRK_LOG_WARN(...)  spdlog::warn(__VA_ARGS__)
    #define DRK_LOG_ERROR(...) spdlog::error(__VA_ARGS__)

#else
    #define DRK_LOGGER_INIT(name)
    #define DRK_LOGGER_SAVE()
    #define DRK_LOG_INFO(...)
    #define DRK_LOG_WARN(...)
    #define DRK_LOG_ERROR(...)
#endif

#endif // DRK_LOG_HPP
