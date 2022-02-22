#ifndef DRK_CORE_BASE_HPP
#define DRK_CORE_BASE_HPP

// Universal includes
#include "Core/Build.hpp"
#include "Core/Debug/Log.hpp"
#include "Core/Debug/Assert.hpp"
#include "Core/Bind.hpp"
#include "Core/SmartPointer.hpp"
#include "Core/Type.hpp"

// Common macros
#define DRK_EXPAND(x) x
#define DRK_STRINGIFY(x) #x
#define DRK_CONCAT_IMPL(a, b) a##b
#define DRK_CONCAT(a, b) DRK_CONCAT_IMPL(a, b)
#define DRK_VA_ARGS(...) ,##__VA_ARGS__

#ifndef APIENTRY
#   if defined(DRK_PLATFORM_WINDOWS)
#       define APIENTRY __stdcall
#   else
#       define APIENTRY
#   endif
#endif
#define DRK_APIENTRY APIENTRY

#if defined(DRK_PLATFORM_WINDOWS)
#   ifndef NOMINMAX
#       define NOMINMAX
#   endif
#endif

namespace DrkCraft
{
    enum ResultStatus
    {
        ResultFailure = 0,
        ResultSuccess = 1
    };
}

#endif // DRK_CORE_BASE_HPP
