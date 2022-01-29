#ifndef DRK_PROFILER_HPP
#define DRK_PROFILER_HPP

#include "BuildSettings.hpp"

#if DRK_PROFILING_ENABLED

    #include "Timer.hpp"
    #include "Time.hpp"

    #include <fstream>
    #include <mutex>

    namespace DrkCraft
    {
        class Profiler;

        class ProfileTimer
        {
        public:
            ProfileTimer(const char* name, const char* cat);
            ~ProfileTimer(void);

            void stop(void);

        private:
            Timer m_timer;
            const char* m_name;
            const char* m_cat;
            bool m_running;
        };

        class Profiler
        {
        public:
            static Profiler& get_instance(void);

            static double get_timestamp(Time::Time time);
            static double get_current_timestamp(void);

        private:
            Profiler(void);
            ~Profiler(void);

        public:
            void begin(const char* name="DrkCraft", const char* file="results.json");
            void end(void);

            bool active(void) const;

            void write_dur_profile(const char* cat, const char* name, double start, double duration);
            void write_inst_profile(const char* cat, const char* name, double ts);
            void write_flow_profile(const char* ph, const char* cat, const char* name, int id, double ts);

            void create_event_profile(const char* name);
            void create_flow_begin_profile(const char* cat, const char* name);
            void create_flow_end_profile(const char* cat, const char* name);

        private:
            void write_header(const char* title, const char* version, const char* time, double timestamp);
            void write_footer(void);

        private:
            static Profiler* s_instance;

            std::ofstream m_outStream;
            const char* m_name;
            bool m_active;

            ProfileTimer* m_sessionTimer;
            std::mutex m_mutex;
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
        ProfileTimer object_profile_timer{name, "object"} // This might not be a good idea

    #define DRK_PROFILE_EVENT(name) \
        Profiler::get_instance().create_event_profile(name)

    #define DRK_PROFILE_FLOW_BEGIN(cat, name) \
        Profiler::get_instance().create_flow_begin_profile(cat, name);

    #define DRK_PROFILE_FLOW_END(cat, name) \
        Profiler::get_instance().create_flow_end_profile(cat, name);

    #define DRK_PROFILE_THREAD_CREATE(name) \
        DRK_PROFILE_FLOW_BEGIN("thread", name);

    #define DRK_PROFILE_THREAD_START(name) \
        DRK_PROFILE_FLOW_END("thread", name);

#else
    #define DRK_PROFILER_BEGIN(name, file)
    #define DRK_PROFILER_END()
    #define DRK_PROFILER_ACTIVE() false

    #define DRK_PROFILE_FUNCTION()
    #define DRK_PROFILE_SCOPE(name)
    #define DRK_PROFILE_OBJECT(name)

    #define DRK_PROFILE_EVENT(name)

    #define DRK_PROFILE_FLOW_BEGIN(cat, name)
    #define DRK_PROFILE_FLOW_END(cat, name)

    #define DRK_PROFILE_THREAD_CREATE(name)
    #define DRK_PROFILE_THREAD_START(name)
#endif

#endif // DRK_PROFILER_HPP
