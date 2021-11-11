#ifndef DRK_BUILD_SETTINGS_HPP
#define DRK_BUILD_SETTINGS_HPP

#if defined(DRK_PLATFORM_WINDOWS)

    #define DRK_PLATFORM_NAME "Windows"

#elif defined(DRK_PLATFORM_LINUX)

    #define DRK_PLATFORM_NAME "Linux"

#else
    #error "Unknown platform detected"
#endif

#if defined(DRK_CONFIG_DEBUG)

    #define DRK_CONFIG_NAME "Debug"

    #define DRK_EN_LOGGING
    #define DRK_EN_ASSERTS
    #define DRK_EN_GL_DEBUG_OUTPUT

#elif defined(DRK_CONFIG_RELEASE)

    #define DRK_CONFIG_NAME "Release"

    #define DRK_EN_ASSERTS // Still enable asserts, but not DRK_DEBUG_BREAK

#else
    #error "Unknown config detected"
#endif

#endif // DRK_BUILD_SETTINGS_HPP
