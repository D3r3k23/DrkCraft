#ifndef DRK_BUILD_HPP
#define DRK_BUILD_HPP

#if defined(DRK_DEBUG)
    #define DRK_CONFIG_NAME Debug

    #define DRK_EN_LOGGING
    #define DRK_EN_ASSERTS
#elif defined(DRK_RELEASE)
    #define DRK_CONFIG_NAME Release
#else
    #error "Unknown config detected"
#endif

#if defined(DRK_PLATFORM_WINDOWS)
    #define DRK_PLATFORM_NAME Windows
#elif defined(DRK_PLATFORM_LINUX)
    #define DRK_PLATFORM_NAME Linux
#else
    #error "Unknown platform detected"
#endif

#endif // DRK_BUILD_HPP
