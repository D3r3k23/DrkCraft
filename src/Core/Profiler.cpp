#include "Profiler.hpp"

#if DRK_PROFILING_ENABLED

    #if defined(DRK_PLATFORM_WINDOWS)
        #include <process.h>
        #define DRK_GET_PID() _getpid()
    #elif defined(DRK_PLATFORM_LINUX)
        #include <unistd.h>
        #define DRK_GET_PID() getpid()
    #else
        #error "Unsupported platform"
    #endif

    #include <fmt/format.h>
    #include <fmt/chrono.h>

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
            if (m_running)
                stop();
        }

        void ProfileTimer::stop(void)
        {
            auto start = Time::duration_cast<Time::Micro<double>>(m_timer.get_start().time_since_epoch());
            Profiler::get_instance().write_profile(m_cat, m_name, start.count(), m_timer.elapsed_microseconds());
            m_running = false;
        }

        //////////////////////////
        //       Profiler       //
        //////////////////////////

        // Static

        Profiler& Profiler::get_instance(void)
        {
            static Profiler s_instance;
            return s_instance;
        }

        double Profiler::get_timestamp(Time::Time time)
        {
            return Time::duration_cast<Time::Micro<double>>(time.time_since_epoch()).count();
        }

        // Instance

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
            std::lock_guard lock(m_mutex);
            if (!active())
            {
                m_active = true;
                m_name = name;

                m_sessionTimer = new ProfileTimer(m_name, "profile");

                m_outStream.open(file);

                auto time = Time::get_system_time();
                double timestamp = get_timestamp(Time::get_global_time());
                write_header(m_name, DRK_VERSION_STRING, fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time)).c_str(), timestamp);
            }
        }

        void Profiler::end(void)
        {
            { std::lock_guard lock(m_mutex); } // Wait for unlock
            if (m_sessionTimer)
                delete m_sessionTimer;

            std::lock_guard lock(m_mutex);
            if (active())
            {
                m_active = false;

                write_footer();
                m_outStream.close();
            }
        }

        bool Profiler::active(void) const
        {
            return m_active;
        }

        // Could potentially switch to writing Duration Events (ph = B, E) to  get
        // more information about what happened before a crash
        void Profiler::write_profile(const char* cat, const char* name, double start, double duration)
        {
            std::stringstream profile;
            profile << ",\n    {";
            profile << fmt::format("\"cat\":\"{}\",",   cat);
            profile << fmt::format("\"name\":\"{}\",",  name);
            profile << fmt::format("\"ts\":{:.2f},",   start);
            profile << fmt::format("\"dur\":{:.2f},", duration);
            profile << fmt::format("\"ph\":\"{}\",",  "X");
            profile << fmt::format("\"pid\":{},",   DRK_GET_PID());
            profile <<             "\"tid\":" <<  std::this_thread::get_id();
            profile << "}";

            std::lock_guard lock(m_mutex);
            m_outStream << profile.str();
            if (FLUSH_ON_WRITE)
                m_outStream.flush();
        }

        void Profiler::write_header(const char* title, const char* version, const char* time, double timestamp)
        {
            std::stringstream header;
            header << std::fixed << std::setprecision(2);
            header << "{\n";
            header << "  ";
            header << "\"title\":\""         << title     << "\",";
            header << "\"version\":\""       << version   << "\",";
            header << "\"time\":\""          << time      << "\",";
            header << "\"timestamp\":"       << timestamp <<   ",";
            header << "\"displayTimeUnit\":" << "\"ms\""  <<   ",\n";
            header << "  \"traceEvents\": [{}";

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
