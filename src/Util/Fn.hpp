#ifndef DRK_UTIL_FN_HPP
#define DRK_UTIL_FN_HPP

#include "Core/Base.hpp"

#include <functional>
#include <concepts>
#include <variant>
#include <utility>

namespace DrkCraft
{
    template <typename ... T>
    struct Visitor : T ...
    {
        using T::operator()...;

        template <typename V>
        auto visit(V&& var)
        {
            return std::visit(*this, std::forward<V>(var));
        }
    };

    template <typename T>
    T lazy_value_or(const Optional<T>& opt, std::invocable auto fn)
    {
        if (opt.has_value())
            return opt.value();
        else
            return fn();
    }

    template <typename T, typename R>
    Optional<R> transform(const Optional<T>& opt, const std::function<R(const T&)>& fn)
    {
        if (opt.has_value())
            return fn(opt.value());
        else
            return {};
    }

    template <typename T>
    auto transform_or(const Optional<T>& opt, const auto& fn, auto defaultValue)
    {
        if (opt.has_value())
            return transform(opt, fn);
        else
            return std::move(defaultValue);
    }

    template <typename R, typename ... Args>
    std::function<R(Args...)> add_prefix(std::function<R(Args...)> fn, std::function<void(void)> prefix)
    {
        return [&](auto&& ... args) -> R
        {
            prefix();
            return fn(std::forward<Args>(args)...);
        };
    }

    template <typename F, typename G, typename ... Args>
    std::function<F(Args...)> compose(std::function<F(G)> f, std::function<G(Args...)> g)
    {
        return [&](auto&& ... args) -> F
        {
            return f(g(args));
        };
    }
}

#endif // DRK_UTIL_FN_HPP
