#ifndef DRK_PID_HPP
#define DRK_PID_HPP

#include "Core/Build.hpp"

#if defined(DRK_PLATFORM_WINDOWS)
    #include <process.h>
    #define DRK_GET_PID() _getpid()
#elif defined(DRK_PLATFORM_LINUX)
    #include <unistd.h>
    #define DRK_GET_PID() getpid()
#else
    #error "Unsupported platform"
#endif

#endif // DRK_PID_HPP
