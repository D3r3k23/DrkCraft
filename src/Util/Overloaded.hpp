#ifndef DRK_UTIL_OVERLOADED_HPP
#define DRK_UTIL_OVERLOADED_HPP

namespace DrkCraft
{
    template <typename ... T>
    struct Overloaded : T ...
    {
        using T::operator()...;
    };
}

#endif // DRK_UTIL_OVERLOADED_HPP
