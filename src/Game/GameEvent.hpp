#ifndef DRK_GAME_EVENT_HPP
#define DRK_GAME_EVENT_HPP

#include "Core/Base.hpp"

#include <vector>

namespace DrkCraft
{
    class GameEventSubscriber
    {

    };

    class GameEventSubject
    {
    public:
        void post_event(const GameEvent& event);

    private:
        std::vector<GameEventSubscriber> m_subscribers;
    };

    class GameEvent
    {
    public:
        static void add_subscriber(const GameEventSubscriber& subscriber);
        static void post(const GameEvent& event);

        static GameEventSubject s_subject;
    };
}

#endif // DRK_GAME_EVENT_HPP
