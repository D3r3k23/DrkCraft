#include "Log.hpp"

#if defined(DRK_EN_LOGGING)

    namespace DrkCraft
    {
        std::string Logger::s_name;

        void Logger::init(std::string_view name)
        {
            s_name = name;

            spdlog::set_level(spdlog::level::STATIC_LOG_LEVEL);

            DRK_LOG_INFO("Logger inintialized: {}", s_name);
        }

        void Logger::close(void)
        {
            DRK_LOG_INFO("Closing Logger: {}", s_name);
        }
    }

#endif
