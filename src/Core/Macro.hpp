#ifndef DRK_CORE_MACRO_HPP
#define DRK_CORE_MACRO_HPP

#define DRK_EXPAND(x) x
#define DRK_STRINGIFY(x) #x
#define DRK_CONCAT_IMPL(a, b) a##b
#define DRK_CONCAT(a, b) DRK_CONCAT_IMPL(a, b)
#define DRK_VA_ARGS(...) ,##__VA_ARGS__

#endif // DRK_CORE_MACRO_HPP
