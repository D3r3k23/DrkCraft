#ifndef DRK_UTIL_FN_HPP
#define DRK_UTIL_FN_HPP

#include "Core/Base.hpp"

#include <functional>

namespace DrkCraft
{
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
