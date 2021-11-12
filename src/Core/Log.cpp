#include "Log.hpp"

#if defined(DRK_EN_LOGGING)

    namespace DrkCraft
    {
        std::string Logger::s_name;

        void Logger::init(std::string_view name)
        {
            s_name = name;

            spdlog::set_level(spdlog::level::trace);

            DRK_LOG_INFO("Log inintialized: {}", s_name);
        }

        void Logger::close(void)
        {
            DRK_LOG_INFO("Closing log: {}", s_name);
        }
    }

#endif
