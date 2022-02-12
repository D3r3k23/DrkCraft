#ifndef DRK_CORE_UTIL_HPP
#define DRK_CORE_UTIL_HPP

#include "Core/Base.hpp"

#include "lib/glm/vec3.hpp"

#include <functional>
#include <utility>

namespace DrkCraft
{
    enum class Direction
    {
        None,

        Left,
        Right,
        Forward,
        Backward,
        Up,
        Down
    };

    vec3 unit_vector(Direction direction);

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

#endif // DRK_CORE_UTIL_HPP
