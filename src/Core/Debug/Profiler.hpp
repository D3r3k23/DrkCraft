#ifndef DRK_CORE_DEBUG_PROFILER_HPP
#define DRK_CORE_DEBUG_PROFILER_HPP

#include "Core/Base.hpp"

#if DRK_PROFILING_ENABLED

    #include "Util/Time.hpp"
    #include "System/Mutex.hpp"

    #include <fstream>

    namespace DrkCraft
    {
        class ProfileTimer
        {
        public:
            ProfileTimer(const char* name, const char* cat);
            ~ProfileTimer(void);

            void stop(void);
            void reset(void);
            bool running(void) const;

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

        public: // Thread safe
            void begin(const char* name="DrkCraft", const char* file="results.json");
            void end(void);

            bool active(void) const;

            void write_dur_profile(const char* cat, const char* name, double start, double duration);
            void write_inst_profile(const char* cat, const char* name, double ts, char scope); // scope: g: global, p: process, t: thread
            void write_flow_profile(const char* cat, const char* name, double ts, char ph, int id); // ph: s: start, f: finish

            void create_event_profile(const char* name, char scope);
            void create_flow_profile(const char* cat, const char* name, char ph);

        private:
            void write_header(const char* title, const char* version, const char* time, double timestamp);
            void write_footer(void);

        private:
            std::ofstream m_outStream;
            const char* m_name;
            bool m_active;

            Optional<ProfileTimer> m_sessionTimer;
            Mutex m_mutex{"profiler_mutex"};
        };
    }

    #define DRK_FUNC_SIG __FUNCTION__

    #define DRK_CLEAN_FUNC_NAME (const char*)(DRK_FUNC_SIG + 10) // Trims "DrkCraft::"

    /////////////////////
    //       API       //
    /////////////////////

    #define DRK_PROFILER_BEGIN(name, file) Profiler::get_instance().begin(name, file)
    #define DRK_PROFILER_END()             Profiler::get_instance().end()
    #define DRK_PROFILER_ACTIVE()          Profiler::get_instance().active()

    // Call at beginning of function (once per function)
    #define DRK_PROFILE_FUNCTION() \
        ProfileTimer DRK_PROFILE_FUNCTION_timer{DRK_CLEAN_FUNC_NAME, "function"}

    // Call in any scope
    #define DRK_PROFILE_SCOPE(name) \
        ProfileTimer DRK_CONCAT(DRK_PROFILE_SCOPE_timer_, __LINE__){name, "scope"}

    // Class member declaration
    #define DRK_OBJECT_PROFILER(name) \
        ProfileTimer DRK_object_profile_timer{name, "object"}

    // Class member declaration
    #define DRK_DEFERRED_OBJECT_PROFILER \
        Optional<ProfileTimer> DRK_deferred_object_profile_timer

    // Class member initialization
    #define DRK_DEFERRED_OBJECT_PROFILER_START(name) \
        DRK_deferred_object_profile_timer.emplace(name, "object")

    // Local event
    #define DRK_PROFILE_EVENT_LOCAL(name) \
        Profiler::get_instance().create_event_profile(name, 't')

    // Global event
    #define DRK_PROFILE_EVENT_GLOBAL(name) \
        Profiler::get_instance().create_event_profile(name, 'g')

    // Call immediately before creating a new thread
    #define DRK_PROFILE_THREAD_CREATE(name) \
        Profiler::get_instance().create_flow_profile("thread", name, 's')

    // Call it beginning of thread
    #define DRK_PROFILE_THREAD(name) \
        Profiler::get_instance().create_flow_profile("thread", name, 'f'); \
        ProfileTimer DRK_PROFILE_THREAD_timer{name, "thread"}

    // Call while waiting on lock
    #define DRK_PROFILE_LOCK(name) \
        Optional<ProfileTimer> DRK_lock_profile_timer{name, "lock"}

#else
    #define DRK_PROFILER_BEGIN(name, file)
    #define DRK_PROFILER_END()
    #define DRK_PROFILER_ACTIVE() false

    #define DRK_PROFILE_FUNCTION()
    #define DRK_PROFILE_SCOPE(name)

    #define DRK_OBJECT_PROFILER(name)
    #define DRK_DEFERRED_OBJECT_PROFILER
    #define DRK_DEFERRED_OBJECT_PROFILER_START(name)

    #define DRK_PROFILE_EVENT_LOCAL(name)
    #define DRK_PROFILE_EVENT_GLOBAL(name)

    #define DRK_PROFILE_THREAD_CREATE(name)
    #define DRK_PROFILE_THREAD(name)

    #define DRK_LOCK_PROFILER(name)
#endif

#endif // DRK_CORE_DEBUG_PROFILER_HPP
