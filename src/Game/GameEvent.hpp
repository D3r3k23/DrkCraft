#ifndef DRK_GAME_EVENT_HPP
#define DRK_GAME_EVENT_HPP

// Do not include directly, use Game/GameEvents.hpp instead

#include "Core/Base.hpp"
#include "GameEventTypes.hpp"

#include <deque>

namespace DrkCraft
{
    struct GameEvent
    {
    public:
        friend class GameEventSubscriber;

        static void post(GameEvent& event);

    private:
        static void subscribe(GameEventSubscriber* subscriber);
        static void unsubscribe(GameEventSubscriber* subscriber);

        GameEvent(void) = default;

    public:
        virtual ~GameEvent(void) = default;

        GameEvent(const GameEvent&) = delete;
        GameEvent(GameEvent&&) = delete;
        GameEvent& operator=(const GameEvent&) = delete;
        GameEvent& operator=(GameEvent&&) = delete;

        virtual const char*    get_name(void) const = 0;
        virtual GameEventFlags get_type(void) const = 0;

        static constexpr GameEventFlags static_type(void) { return 0; }

    private:
        static std::deque<GameEventSubscriber*> s_subscribers;
    };

    class GameEventSubscriber
    {
    public:
        GameEventSubscriber(GameEventFlags subscriptions);
        ~GameEventSubscriber(void);

        virtual void on_game_event(GameEvent& event) = 0;

        GameEventFlags game_event_subscriptions(void) const;
        bool subscribed(const GameEvent& event) const;

    protected:
        GameEventFlags m_gameEventSubscriptions;
    };

    template <typename E>
    concept GameEventConcept = std::derived_from<E, GameEvent>;

    template <typename E>
    using GameEventHandlerFn = std::function<void(const E&)>;

    class GameEventDispatcher
    {
    public:
        GameEventDispatcher(const GameEvent& event)
          : event(event)
        { }

        template <GameEventConcept E>
        void dispatch(const GameEventHandlerFn<E>& handler)
        {
            if (event.get_type() == E::static_type())
                handler(static_cast<const E&>(event));
        }

    private:
        const GameEvent& event;
    };
}

#endif // DRK_GAME_EVENT_HPP
