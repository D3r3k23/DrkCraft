#ifndef DRK_PROFILER_HPP
#define DRK_PROFILER_HPP

#include "BuildSettings.hpp"

#if defined(DRK_EN_PROFILE)

    #include "Base.hpp"
    #include "Timer.hpp"
    #include "Time.hpp"

    #include <fstream>

    namespace DrkCraft
    {
        class Profiler
        {
        public:
            static Profiler& get_instance(void);

            static double get_timestamp(Time::Time time);

        private:
            Profiler(void);
            ~Profiler(void);

        public:
            void begin(const char* name="DrkCraft", const char* file="results.json");
            void end(void);

            bool active(void) const;

            void write_profile(const char* cat, const char* name, double start, double duration);

        private:
            void write_header(const char* title, const char* version, const char* time, double timestamp);
            void write_footer(void);

        private:
            static Profiler* s_instance;

            std::ofstream m_outStream;
            const char* m_name;
            bool m_active;
        };

        class ProfileTimer
        {
        public:
            // Currently only tracks the duration from construction to destruction
            // of the instance. Could potentially expand the API to allow for an
            // explicit call to stop()
            ProfileTimer(const char* name, const char* cat);
            ~ProfileTimer(void);

        private:
            void stop(void);

        private:
            Timer m_timer;
            const char* m_name;
            const char* m_cat;
            bool m_running;
        };
    }

    #define DRK_FUNC_SIG __FUNCTION__

    #define DRK_CLEAN_FUNC_NAME (const char*)(DRK_FUNC_SIG + 10) // Trims "DrkCraft::"

    #define DRK_PROFILER_BEGIN(name, file) Profiler::get_instance().begin(name, file)
    #define DRK_PROFILER_END()             Profiler::get_instance().end()
    #define DRK_PROFILER_ACTIVE()          Profiler::get_instance().active()

    // At the moment we can't have two ProfileTimers in the same scope
    // Could add line # to timer name?

    #define DRK_PROFILE_FUNCTION() \
        ProfileTimer function_profile_timer{DRK_CLEAN_FUNC_NAME, "function"}

    #define DRK_PROFILE_SCOPE(name) \
        ProfileTimer scope_profile_timer{name, "scope"}

    #define DRK_PROFILE_OBJECT(name) \
        ProfileTimer object_profile_timer{name, "object"}

#else
    #define DRK_PROFILER_BEGIN(name, file)
    #define DRK_PROFILER_END()
    #define DRK_PROFILER_ACTIVE() false

    #define DRK_PROFILE_FUNCTION()
    #define DRK_PROFILE_SCOPE(name)
    #define DRK_PROFILE_OBJECT(name)
#endif

#endif // DRK_PROFILER_HPP
