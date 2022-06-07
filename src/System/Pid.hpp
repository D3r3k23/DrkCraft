#ifndef DRK_PID_HPP
#define DRK_PID_HPP

#include "Core/Build.hpp"

#if DRK_WINDOWS_ENABLED
#   include <process.h>
#   define DRK_GET_PID() _getpid()
#elif DRK_LINUX_ENABLED
#   include <unistd.h>
#   define DRK_GET_PID() getpid()
#else
#   error "Unsupported platform"
#endif

#endif // DRK_PID_HPP
