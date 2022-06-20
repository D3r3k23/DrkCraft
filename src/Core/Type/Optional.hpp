#ifndef DRK_CORE_TYPE_OPTIONAL_HPP
#define DRK_CORE_TYPE_OPTIONAL_HPP

#include <optional>
#include <functional>
#include <utility>
#include <compare>

namespace DrkCraft
{
    template <typename T>
    class Optional
    {
    private:
        using OptionalType = std::optional<T>;
        OptionalType m_optional;

    public:
        constexpr Optional(auto&& ... args)
          : m_optional(std::forward<decltype(args)>(args)...)
        { }

        constexpr Optional(std::in_place_t, auto&& ... args)
          : m_optional(std::in_place, std::forward<decltype(args)>(args)...)
        { }

        constexpr Optional& operator=(auto& a)
        {
            m_optional = std::forward<decltype(a)>(a);
            return *this;
        }

        template <typename ... Args>
        T& emplace(Args&& ... args)
        {
            return m_optional.emplace(std::forward<Args>(args)...);
        }

        void reset(void)
        {
            m_optional.reset();
        }

        constexpr bool has_value(void) const
        {
            return m_optional.has_value();
        }

        operator bool(void) const
        {
            return has_value();
        }

        constexpr const T& value(void) const
        {
            return m_optional.value();
        }

        constexpr T value_or(T&& defaultValue) const
        {
            if (has_value())
                return value();
            else
                return defaultValue;
        }

        constexpr T lazy_value_or(auto&& fn) const
        {
            if (has_value())
                return value();
            else
                return fn();
        }

        constexpr const T& operator*(void) const
        {
            return *m_optional;
        }

        constexpr T& operator*(void)
        {
            return *m_optional;
        }

        constexpr const T* operator->(void) const
        {
            return m_optional.operator->();
        }

        constexpr T* operator->(void)
        {
            return m_optional.operator->();
        }

        template <typename R>
        Optional<R> transform(const std::function<R(const T&)>& fn)
        {
            if (has_value())
                return fn(*m_optional);
            else
                return {};
        }

        template <typename R>
        R transform_or(const std::function<R(const T&)>& fn, R&& defaultValue)
        {
            if (has_value())
                return transform(*m_optional, fn);
            else
                return defaultValue;
        }

        template <typename U>
        friend constexpr auto operator<=>(const Optional<T>& opt1, const Optional<U>& opt2)
            { return opt1.m_optional <=> opt2.m_optional; }

        template <typename V>
        friend constexpr auto operator<=>(const Optional<T>& opt, const V& value)
            { return opt.m_optional <=> value; }

        template <typename V>
        friend constexpr auto operator<=>(const V& value, const Optional& opt)
            { return opt <=> value; }

        friend constexpr bool operator==(const Optional& opt1, const Optional& opt2)
            { return opt1.m_optional == opt2.m_optional; }

        void swap(Optional& other)
        {
            m_optional.swap(other.m_optional);
        }

        auto hash(void) const
        {
            return std::hash<OptionalType>(m_optional);
        }
    };

    template <typename T>
    void swap(Optional<T>& opt1, Optional<T>& opt2)
    {
        opt1.swap(opt2);
    }

    template <typename T>
    constexpr Optional<std::decay<T>> make_optional(T&& value)
    {
        return Optional<std::decay_t<T>>(std::forward<T>(value));
    }

    template <typename T, typename ... Args>
    constexpr Optional<T> make_optional(Args&& ... args)
    {
        return Optional<T>(std::in_place, std::forward<Args>(args)...);
    }
}

namespace std
{
    template <typename T>
    struct hash<DrkCraft::Optional<T>>
    {
        auto operator()(const DrkCraft::Optional<T>& optional) const
        {
            return optional.hash();
        }
    };
}

#endif // DRK_CORE_TYPE_OPTIONAL_HPP
