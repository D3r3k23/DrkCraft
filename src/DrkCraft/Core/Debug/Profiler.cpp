#include "Profiler.hpp"

#if DRK_PROFILING_ENABLED

    #include "Util/File.hpp"
    #include "System/Thread.hpp"
    #include "System/Lock.hpp"
    #include "System/Pid.hpp"

    #include "lib/fs.hpp"
    #include "lib/string.hpp"

    #include <fmt/format.h>
    #include <fmt/chrono.h>

    #include <sstream>

    const bool FLUSH_ON_WRITE = false;

    namespace DrkCraft
    {
        using ProfilerLock = Lock<DISABLE_PROFILE>;

        //////////////////////////////
        //       ProfileTimer       //
        //////////////////////////////

        ProfileTimer::ProfileTimer(const char* name, const char* cat)
          : m_name(name),
            m_cat(cat),
            m_running(true)
        { }

        ProfileTimer::~ProfileTimer(void)
        {
            if (running())
                stop();
        }

        void ProfileTimer::stop(void)
        {
            const auto elapsed = m_timer.elapsed_microseconds();
            const double start = Profiler::get_timestamp(m_timer.get_start());

            Profiler::get_instance().write_dur_profile(m_cat, m_name, start, elapsed);
            m_running = false;
        }

        void ProfileTimer::reset(void)
        {
            m_timer.reset();
        }

        bool ProfileTimer::running(void) const
        {
            return m_running;
        }

        //////////////////////////
        //       Profiler       //
        //////////////////////////

        ////// Static //////

        Profiler& Profiler::get_instance(void)
        {
            static Profiler s_instance;
            return s_instance;
        }

        double Profiler::get_timestamp(Time::Time time)
        {
            return Time::as_duration<Time::Micro<double>>(time).count();
        }

        double Profiler::get_current_timestamp(void)
        {
            return get_timestamp(Time::get_program_time());
        }

        ////// Instance //////

        Profiler::Profiler(void)
          : m_active(false)
        { }

        Profiler::~Profiler(void)
        {
            if (active())
                end();
        }

        void Profiler::begin(const char* name, const char* file)
        {
            if (!active())
            {
                ProfilerLock lock(m_mutex);

                m_active = true;
                m_name = name;

                m_outStream.open(file);

                const auto time = Time::get_system_time();
                const auto time_str = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));
                const double timestamp = get_timestamp(Time::Clock::now());
                write_header(m_name, DRK_VERSION_STRING, time_str.c_str(), timestamp);

                m_sessionTimer.emplace(m_name, "profiler_session");
            }
        }

        void Profiler::end(void)
        {
            if (active())
            {
                if (m_sessionTimer && m_sessionTimer->running())
                    m_sessionTimer->stop();

                ProfilerLock lock(m_mutex);

                write_footer();

                m_active = false;
                m_outStream.close();
            }
        }

        bool Profiler::active(void) const
        {
            return m_active;
        }

        // Could potentially switch to writing Duration Events (ph = B, E) to get
        // more information about what happened before a crash
        void Profiler::write_dur_profile(const char* cat, const char* name, double start, double duration)
        {
            if (active())
            {
                std::stringstream profile;
                profile << ",\n    {";
                profile << fmt::format("\"ph\":\"{}\",", "X");
                profile << fmt::format("\"cat\":\"{}\",", cat);
                profile << fmt::format("\"name\":\"{}\",", name);
                profile << fmt::format("\"ts\":{:.2f},",   start);
                profile << fmt::format("\"dur\":{:.2f},", duration);
                profile << fmt::format("\"pid\":{},",   DRK_GET_PID());
                profile <<             "\"tid\":" <<  Thread<>::This::id();
                profile << "}";

                ProfilerLock lock(m_mutex);
                m_outStream << profile.str();

                if (FLUSH_ON_WRITE)
                    m_outStream.flush();
            }
        }

        void Profiler::write_inst_profile(const char* cat, const char* name, double ts, char scope)
        {
            if (active())
            {
                std::stringstream profile;
                profile << ",\n    {";
                profile << fmt::format("\"ph\":\"{}\",", "i");
                profile << fmt::format("\"cat\":\"{}\",",  cat);
                profile << fmt::format("\"name\":\"{}\",", name);
                profile << fmt::format("\"ts\":{:.2f},",    ts);
                profile << fmt::format("\"s\":\"{}\",",  scope);
                profile << fmt::format("\"pid\":{},",   DRK_GET_PID());
                profile <<             "\"tid\":" <<  Thread<>::This::id();
                profile << "}";

                ProfilerLock lock(m_mutex);
                m_outStream << profile.str();

                if (FLUSH_ON_WRITE)
                    m_outStream.flush();
            }
        }

        void Profiler::write_flow_profile(const char* cat, const char* name, double ts, char ph, int id)
        {
            if (active())
            {
                std::stringstream profile;
                profile << ",\n    {";
                profile << fmt::format("\"ph\":\"{}\",",    ph);
                profile << fmt::format("\"cat\":\"{}\",",   cat);
                profile << fmt::format("\"name\":\"{}\",", name);
                profile << fmt::format("\"id\":\"{}\",",   id);
                profile << fmt::format("\"ts\":{:.2f},",  ts);
                profile << fmt::format("\"pid\":{},",   DRK_GET_PID());
                profile <<             "\"tid\":" <<  Thread<>::This::id();
                profile << "}";

                ProfilerLock lock(m_mutex);
                m_outStream << profile.str();

                if (FLUSH_ON_WRITE)
                    m_outStream.flush();
            }
        }

        void Profiler::create_event_profile(const char* name, char scope)
        {
            write_inst_profile("event", name, get_current_timestamp(), scope);
        }

        void Profiler::create_flow_profile(const char* cat, const char* name, char ph)
        {
            int id = std::hash<string>{}(name);
            write_flow_profile(cat, name, get_current_timestamp(), id, ph);
        }

        void Profiler::write_header(const char* title, const char* version, const char* time, double timestamp)
        {
            std::stringstream header;
            header << "{\n  ";
            header << fmt::format("\"title\":\"{}\",", title);
            header << fmt::format("\"version\":\"{}\",", version);
            header << fmt::format("\"time\":\"{}\",", time);
            header << fmt::format("\"timestamp\":{},", timestamp);
            header << fmt::format("\"displayTimeUnit\":\"{}\",\n", "ms");
            header << fmt::format("  \"traceEvents\": [{{}}");

            m_outStream << header.str();
            m_outStream.flush();
        }

        void Profiler::write_footer(void)
        {
            string footer =
                "\n"
                "  ]\n"
                "}\n";

            m_outStream << footer;
            m_outStream.flush();
        }
    }

#endif
