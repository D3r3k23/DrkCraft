#include "Log.hpp"

#if defined(DRK_EN_LOGGING)

    namespace DrkCraft
    {
        std::string Logger::s_name;

        void Logger::init(std::string_view name)
        {
            s_name = name;
            DRK_LOG_INFO("{} log init", name);
        }

        void Logger::save(void)
        {

        }
    }

#endif
