#ifndef DRK_CORE_BASE_HPP
#define DRK_CORE_BASE_HPP

// Universal includes
#include "Core/BuildSettings.hpp"
#include "Core/Debug/Log.hpp"
#include "Core/Debug/Assert.hpp"
#include "Core/Bind.hpp"
#include "lib/smart_pointer.hpp"
#include "lib/type.hpp"

// Common macros
#define DRK_EXPAND(x) x
#define DRK_STRINGIFY(x) #x
#define DRK_CONCAT_IMPL(a, b) a##b
#define DRK_CONCAT(a, b) DRK_CONCAT_IMPL(a, b)
#define DRK_VA_ARGS(...) ,##__VA_ARGS__

namespace DrkCraft
{
    enum ResultType
    {
        ResultSuccess = 0,
        ResultFailure = 1
    };
}

#endif // DRK_CORE_BASE_HPP
