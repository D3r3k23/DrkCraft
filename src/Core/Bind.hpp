#ifndef DRK_CORE_BIND_HPP
#define DRK_CORE_BIND_HPP

#include <utility>

// Bind member function to lambda
#define DRK_BIND_FN(fn) [this](auto&& ... args) -> auto \
{                                                      \
    return fn(std::forward<decltype(args)>(args)...); \
}

#endif // DRK_CORE_BIND_HPP
