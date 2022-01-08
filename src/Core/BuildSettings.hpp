#ifndef DRK_BUILD_SETTINGS_HPP
#define DRK_BUILD_SETTINGS_HPP

#if defined(DRK_PLATFORM_WINDOWS)
    #define DRK_PLATFORM_NAME "Windows"
#elif defined(DRK_PLATFORM_LINUX)
    #define DRK_PLATFORM_NAME "Linux"
#else
    #error "Unknown platform"
#endif

#if defined(DRK_CONFIG_DEBUG)
    #define DRK_CONFIG_NAME "Debug"
    #define DRK_STATIC_LOG_LEVEL trace
#elif defined(DRK_CONFIG_RELEASE)
    #define DRK_CONFIG_NAME "Release"
    #define DRK_STATIC_LOG_LEVEL warn
#else
    #error "Unknown config"
#endif

#if defined(DRK_EN_LOGGING)
    #define DRK_LOGGING_ENABLED true
#else
    #define DRK_LOGGING_ENABLED false
#endif

#if defined(DRK_EN_ASSERTS)
    #define DRK_ASSERTS_ENABLED true
#else
    #define DRK_ASSERTS_ENABLED false
#endif

#if defined(DRK_EN_PROFILE)
    #define DRK_PROFILING_ENABLED true
#else
    #define DRK_PROFILING_ENABLED false
#endif

#if defined(DRK_EN_DEV_MODE)
    #define DRK_DEV_MODE_ENABLED true
#else
    #define DRK_DEV_MODE_ENABLED false
#endif

#endif // DRK_BUILD_SETTINGS_HPP
