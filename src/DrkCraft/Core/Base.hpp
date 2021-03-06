#ifndef DRK_CORE_BASE_HPP
#define DRK_CORE_BASE_HPP

#include "Core/Build.hpp"
#include "Core/Type.hpp"
#include "Core/Bind.hpp"
#include "Core/Macro.hpp"
#include "Core/Result.hpp"

#include "Core/Debug/Log.hpp"
#include "Core/Debug/Assert.hpp"

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
