#ifndef DRK_UTIL_FN_HPP
#define DRK_UTIL_FN_HPP

#include "Core/Base.hpp"

#include <functional>
#include <concepts>
#include <optional>
#include <utility>

// Functional programming utilities

namespace DrkCraft
{
    template <typename ... T>
    struct Overload : T ...
    {
        using T::operator()...;
    };

    template <typename T>
    T lazy_value_or(const std::optional<T>& opt, auto fn)
    {
        if (opt.has_value())
            return opt.value();
        else
            return fn();
    }

    template <typename R, typename ... Args>
    std::function<R(Args...)> add_prefix(std::function<R(Args...)> fn, std::function<void(void)> prefix)
    {
        return [&](auto&& ... args) -> R
        {
            prefix();
            fn(std::forward<Args>(args)...);
        };
    }
}

#endif // DRK_UTIL_FN_HPP
