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
    // Logging enabled
#else
    // Logging disabled
#endif

#if defined(DRK_EN_ASSERTS)
    // Asserts enabled
#else
    // Asserts disabled
#endif

#if defined(DRK_EN_PROFILE)
    // Profiling enabled
#else
    // Profiling disabled
#endif

#if defined(DRK_EN_DEV_MODE)
    // Dev mode enabled
#else
    // Dev mode disabled
#endif

#endif // DRK_BUILD_SETTINGS_HPP
