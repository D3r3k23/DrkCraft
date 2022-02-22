#include "Profiler.hpp"

#if DRK_PROFILING_ENABLED

    #include "Util/File.hpp"
    #include "Util/Pid.hpp"

    #include "lib/fs.hpp"

    #include <fmt/format.h>
    #include <fmt/chrono.h>

    #include <string>
    #include <sstream>
    #include <thread>

    const bool FLUSH_ON_WRITE = false;

    namespace DrkCraft
    {
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
            DRK_PROFILE_FUNCTION();

            if (!active())
            {
                m_sessionTimer = make_ptr<ProfileTimer>(m_name, "profile");

                m_active = true;
                m_name = name;

                ensure_dir_exists(fs::path(file).parent_path());
                m_outStream.open(file);

                auto time = Time::get_system_time();
                double timestamp = get_timestamp(Time::get_program_start_time());
                write_header(m_name, DRK_VERSION_STRING, fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time)).c_str(), timestamp);
            }
        }

        void Profiler::end(void)
        {
            {
                DRK_PROFILE_FUNCTION();

                if (m_sessionTimer->running())
                    m_sessionTimer->stop();

                if (active())
                {
                    m_mutex.lock();
                    write_footer();
                }
            }
            if (active())
            {
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
                profile <<             "\"tid\":" <<  std::this_thread::get_id();
                profile << "}";

                std::lock_guard lock(m_mutex);
                m_outStream << profile.str();

                if (FLUSH_ON_WRITE)
                    m_outStream.flush();
            }
        }

        void Profiler::write_inst_profile(const char* cat, const char* name, double ts)
        {
            if (active())
            {
                std::stringstream profile;
                profile << ",\n    {";
                profile << fmt::format("\"ph\":\"{}\",", "i");
                profile << fmt::format("\"cat\":\"{}\",",  cat);
                profile << fmt::format("\"name\":\"{}\",", name);
                profile << fmt::format("\"ts\":{:.2f},",   ts);
                profile << fmt::format("\"s\":\"{}\",",  "t");
                profile << fmt::format("\"pid\":{},",   DRK_GET_PID());
                profile <<             "\"tid\":" <<  std::this_thread::get_id();
                profile << "}";

                std::lock_guard lock(m_mutex);
                m_outStream << profile.str();

                if (FLUSH_ON_WRITE)
                    m_outStream.flush();
            }
        }

        void Profiler::write_flow_profile(const char* ph, const char* cat, const char* name, int id, double ts)
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
                profile <<             "\"tid\":" <<  std::this_thread::get_id();
                profile << "}";

                std::lock_guard lock(m_mutex);
                m_outStream << profile.str();

                if (FLUSH_ON_WRITE)
                    m_outStream.flush();
            }
        }

        void Profiler::create_event_profile(const char* name)
        {
            write_inst_profile("event", name, get_current_timestamp());
        }

        void Profiler::create_flow_begin_profile(const char* cat, const char* name)
        {
            int id = std::hash<std::string>{}(name);
            write_flow_profile("s", cat, name, id, get_current_timestamp());
        }

        void Profiler::create_flow_end_profile(const char* cat, const char* name)
        {
            int id = std::hash<std::string>{}(name);
            write_flow_profile("f", cat, name, id, get_current_timestamp());
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
            std::string footer =
                "\n"
                "  ]\n"
                "}\n";

            m_outStream << footer;
            m_outStream.flush();
        }
    }

#endif
