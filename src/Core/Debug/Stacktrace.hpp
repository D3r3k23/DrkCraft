#ifndef DRK_CORE_DEBUG_STACKTRACE_HPP
#define DRK_CORE_DEBUG_STACKTRACE_HPP

#include "Core/Build.hpp"
#include "Core/Type.hpp"

#if DRK_DEBUG_ENABLED
#   define DRK_STACKTRACE_ENABLED 1
#else
#   define DRK_STACKTRACE_ENABLED 0
#endif

#if DRK_STACKTRACE_ENABLED
    #include "backward/backward.hpp"
    #include <iostream>

    #define DRK_PRINT_STACKTRACE()               \
        Stacktrace DRK_PRINT_STACKTRACE_var_st;   \
        DRK_PRINT_STACKTRACE_var_st.load_here(16); \
        print(DRK_PRINT_STACKTRACE_var_st)
#else
    #define DRK_PRINT_STACKTRACE()
#endif

namespace DrkCraft
{
#if DRK_STACKTRACE_ENABLED
    using Stacktrace = backward::StackTrace;
    using StacktracePrinter = backward::Printer;

    inline namespace detail
    {
        void print(const StackTrace& st)
        {
            StacktracePrinter printer;
            printer.color_mode = backward::ColorMode::automatic;
            printer.object = true;
            printer.address = true;
            printer.print(st, std::cout);
        }
    }
#endif
}

#endif // DRK_CORE_DEBUG_STACKTRACE_HPP
