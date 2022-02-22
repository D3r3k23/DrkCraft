#ifndef DRK_UTIL_FN_HPP
#define DRK_UTIL_FN_HPP

#include "Core/Base.hpp"

#include <functional>
#include <utility>

namespace DrkCraft
{
    template <typename R, typename ... Args>
    std::function<R(Args...)> add_prefix_function(std::function<R(Args...)> fn, std::function<void(void)> prefix)
    {
        return [&](auto&& ... args) -> R
        {
            prefix();
            fn(std::forward<Args>(args)...);
        };
    }
}

#endif // DRK_UTIL_FN_HPP
