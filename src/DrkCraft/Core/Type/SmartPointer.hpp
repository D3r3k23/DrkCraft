#ifndef DRK_CORE_TYPE_SMART_POINTER_HPP
#define DRK_CORE_TYPE_SMART_POINTER_HPP

#include <memory>
#include <utility>

namespace DrkCraft
{
    // Lifetime-owning pointer
    template <typename T, typename D=std::default_delete<T>>
    using Ptr = std::unique_ptr<T, D>;

    template <typename T, typename ... Args>
    constexpr Ptr<T> make_ptr(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    // Reference-counted pointer
    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Ref<T> make_ref(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#endif // DRK_CORE_TYPE_SMART_POINTER_HPP
