#ifndef DRK_PROFILER_HPP
#define DRK_PROFILER_HPP

#include "Base.hpp"
#include "Timer.hpp"

#include <fstream>

namespace DrkCraft
{
    class Profiler
    {
    public:
        Profiler(void);
        static Profiler& get_instance(void);

        static const char* func_name(const char* funcSig);

        void begin(const char* file="results.json");
        void end(void);

        void write_profile(const char* cat, const char* name, float start, float duration);

    private:
        void write_header(void);
        void write_footer(void);

    private:
        static Profiler s_instance;

        std::ofstream m_outStream;
        bool m_active;
    };

    class ProfileTimer
    {
    public:
        ProfileTimer(const char* name, const char* cat);
        ~ProfileTimer(void);

    private:
        Timer m_timer;
        const char* m_name;
        const char* m_cat;
    };
}

#if defined(DRK_EN_PROFILE)

    #define DRK_FUNC_SIG __FUNCTION__ // Works with MSVC

    #define DRK_PROFILER_BEGIN(file) Profiler::get_instance().begin(file)
    #define DRK_PROFILER_END()       Profiler::get_instance().end()

    #define DRK_PROFILE_FUNCTION() \
        ProfileTimer function_profile_timer(Profiler::func_name(DRK_FUNC_SIG), "function")

    #define DRK_PROFILE_SCOPE(name) \
        ProfileTimer scope_profile_timer(name, "scope")

#else
    #define DRK_PROFILER_BEGIN(file)
    #define DRK_PROFILER_END()

    #define DRK_PROFILE_FUNCTION()
    #define DRK_PROFILE_SCOPE(name)
#endif

#endif // DRK_PROFILER_HPP
