#include "Stacktrace.hpp"

#if DRK_TRACKTRACE_ENABLED
#   include <iostream>
#endif

namespace DrkCraft
{
#if DRK_STACKTRACE_ENABLED
    static backward::SignalHandling sh;

    void print(const Stacktrace& st)
    {
        StacktracePrinter printer;
        printer.color_mode = backward::ColorMode::automatic; // always
        printer.object = true;
        printer.address = true;
        printer.print(st, std::cout);
    }
#endif
}
