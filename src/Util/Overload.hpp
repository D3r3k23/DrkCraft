#ifndef DRK_UTIL_OVERLOAD_HPP
#define DRK_UTIL_OVERLOAD_HPP

namespace DrkCraft
{
    template <typename ... T>
    struct Overload : T ...
    {
        using T::operator()...;
    };
}

#endif // DRK_UTIL_OVERLOAD_HPP
