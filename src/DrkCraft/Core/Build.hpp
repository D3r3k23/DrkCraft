#ifndef DRK_CORE_BUILD_HPP
#define DRK_CORE_BUILD_HPP

#include "Core/Version.hpp"

// Required compile definitions:
//
// DRK_VERSION_STRING "x.x"
// DRK_PLATFORM_WINDOWS | DRK_PLATFORM_LINUX
// DRK_CONFIG_DEBUG | DRK_CONFIG_RELEASE

namespace DrkCraft::Build
{
    inline const Version VERSION(DRK_VERSION_STRING);

    void log_info(void);
}

// Platform

#if defined(DRK_PLATFORM_WINDOWS)
#   define DRK_PLATFORM_NAME "Windows"
#elif defined(DRK_PLATFORM_LINUX)
#   define DRK_PLATFORM_NAME "Linux"
#else
#   error "Unknown platform"
#endif

#if defined(DRK_PLATFORM_WINDOWS)
#   define DRK_WINDOWS_ENABLED 1
#else
#   define DRK_WINDOWS_ENABLED 0
#endif

#if defined(DRK_PLATFORM_LINUX)
#   define DRK_LINUX_ENABLED 1
#else
#   define DRK_LINUX_ENABLED 0
#endif

// Config

#if defined(DRK_CONFIG_DEBUG)
#   define DRK_CONFIG_NAME "Debug"
#elif defined(DRK_CONFIG_RELEASE)
#   define DRK_CONFIG_NAME "Release"
#else
#   error "Unknown config"
#endif

#if defined(DRK_CONFIG_DEBUG)
#   define DRK_DEBUG_ENABLED 1
#else
#   define DRK_DEBUG_ENABLED 0
#endif

#if defined(DRK_CONFIG_RELEASE)
#   define DRK_RELEASE_ENABLED 1
#else
#   define DRK_RELEASE_ENABLED 0
#endif

////// Optional features //////

// Logging

#if defined(DRK_EN_LOGGING)
#   define DRK_LOGGING_ENABLED 1
#else
#   define DRK_LOGGING_ENABLED 0
#endif

// Asserts

#if defined(DRK_EN_ASSERTS)
#   define DRK_ASSERTS_ENABLED 1
#else
#   define DRK_ASSERTS_ENABLED 0
#endif

// Stacktrace

#if defined(DRK_EN_STACKTRACE)
#   define DRK_STACKTRACE_ENABLED 1
#else
#   define DRK_STACKTRACE_ENABLED 0
#endif

// Profiling

#if defined(DRK_EN_PROFILING)
#   define DRK_PROFILING_ENABLED 1
#else
#   define DRK_PROFILING_ENABLED 0
#endif

// Dev Mode

#if defined(DRK_EN_DEV_MODE)
#   define DRK_DEV_MODE_ENABLED 1
#else
#   define DRK_DEV_MODE_ENABLED 0
#endif

#endif // DRK_CORE_BUILD_HPP
