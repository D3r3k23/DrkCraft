#include "Log.hpp"

#if defined(DRK_EN_LOGGING)

    #include "Application/Events.hpp"

    namespace DrkCraft
    {
        std::string Logger::s_name;

        void Logger::init(std::string_view name)
        {
            s_name = name;

            spdlog::set_level(spdlog::level::DRK_STATIC_LOG_LEVEL);

            DRK_LOG_INFO("Logger inintialized: {}", s_name);
        }

        void Logger::close(void)
        {
            DRK_LOG_INFO("Closing Logger: {}", s_name);
        }

        void Logger::log_event(const Event& event)
        {
            switch (event.get_type())
            {
                case EventType::WindowResize : break;
                case EventType::WindowMoved  : break;
                case EventType::MouseMoved   : break;
                case EventType::CharTyped    : break;
                case EventType::KeyHeld      : break;
                default:
                    spdlog::info("[Event] ({}handled) {}", event.handled() ? "un" : "", std::string(event));
            }
        }
    }

#endif
