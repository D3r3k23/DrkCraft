#ifndef DRK_GAME_GAME_EVENT_HPP
#define DRK_GAME_GAME_EVENT_HPP

// Do not include directly, use Game/GameEvents.hpp instead

#include "Core/Base.hpp"
#include "Game/GameEventTypes.hpp"

#include <deque>
#include <concepts>
#include <type_traits>
#include <functional>

namespace DrkCraft
{
    struct GameEvent
    {
        friend class GameEventSubscriber;

    public:
        static void post(const GameEvent& event);

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

        virtual const char*   get_name(void) const = 0;
        virtual GameEventType get_type(void) const = 0;

        static constexpr GameEventCategory static_type(void) { return GameEventCategory::GameEvent; }

    private:
        static std::deque<GameEventSubscriber*> s_subscribers;
    };

    class GameEventSubscriber
    {
    public:
        GameEventSubscriber(void);
        ~GameEventSubscriber(void);

        virtual void on_game_event(const GameEvent& event) = 0;
    };

    template <typename E>
    concept GameEventConcept = std::derived_from<E, GameEvent>;

    template <typename E>
    concept AbstractGameEventConcept = GameEventConcept<E> && !std::is_final<E>::value;

    template <GameEventConcept E>
    using GameEventHandlerFn = std::function<void(const E&)>;

    template <AbstractGameEventConcept E1>
    class GameEventDispatcher
    {
    public:
        GameEventDispatcher(const E1& event)
          : event(event)
        { }

        template <GameEventConcept E2> requires std::derived_from<E2, E1>
        void dispatch(const GameEventHandlerFn<E2>& handler)
        {
            if (event == E2::static_type())
                handler(static_cast<const E2&>(event));
        }

    private:
        const E1& event;
    };
}

#endif // DRK_GAME_GAME_EVENT_HPP
