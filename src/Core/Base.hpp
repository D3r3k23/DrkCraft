#ifndef DRK_CORE_BASE_HPP
#define DRK_CORE_BASE_HPP

#include "Core/Type.hpp"
#include "Core/Build.hpp"
#include "Core/Bind.hpp"
#include "Core/SmartPointer.hpp"
#include "Core/Result.hpp"

#include "Core/Debug/Log.hpp"
#include "Core/Debug/Assert.hpp"

#define DRK_EXPAND(x) x
#define DRK_STRINGIFY(x) #x
#define DRK_CONCAT_IMPL(a, b) a##b
#define DRK_CONCAT(a, b) DRK_CONCAT_IMPL(a, b)
#define DRK_VA_ARGS(...) ,##__VA_ARGS__

#ifndef APIENTRY
#   if DRK_WINDOWS_ENABLED
#       define APIENTRY __stdcall
#   else
#       define APIENTRY
#   endif
#endif
#define DRK_APIENTRY APIENTRY

#if DRK_WINDOWS_ENABLED
#   ifndef NOMINMAX
#       define NOMINMAX
#   endif
#endif

#endif // DRK_CORE_BASE_HPP
