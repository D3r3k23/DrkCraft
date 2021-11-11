#include "Log.hpp"

#if defined(DRK_EN_LOGGING)

    namespace DrkCraft
    {
        void Logger::init(void)
        {
            spdlog::set_level(spdlog::level::trace);
        }

        void Logger::save(void)
        {

        }
    }

#endif
