#ifndef DRK_CORE_DEBUG_STACKTRACE_HPP
#define DRK_CORE_DEBUG_STACKTRACE_HPP

#include "Core/Build.hpp"
#include "Core/Type.hpp"

#if DRK_STACKTRACE_ENABLED
    #include "backward/backward.hpp"

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

    void print(const Stacktrace& st);
#endif
}

#endif // DRK_CORE_DEBUG_STACKTRACE_HPP
