#include "Profiler.hpp"

#if defined(DRK_EN_PROFILE)

    #if defined(DRK_PLATFORM_WINDOWS)
        #include <processthreadsapi.h>
        #define DRK_GET_PID() GetCurrentProcessId()
    #elif defined(DRK_PLATFORM_LINUX)
        #include <unistd.h>
        #define DRK_GET_PID() getpid()
    #else
        #error "Unsupported platform"
    #endif

    #include <format>
    #include <sstream>
    #include <iomanip>
    #include <thread>

    const bool FLUSH_ON_WRITE = false;

    namespace DrkCraft
    {
        Profiler& Profiler::get_instance(void)
        {
            static Profiler s_instance;
            return s_instance;
        }

        double Profiler::get_timestamp(Time::Time time)
        {
            return Time::duration_cast<Time::Micro<double>>(time.time_since_epoch()).count();
        }

        Profiler::Profiler(void)
          : m_active(false)
        { }

        Profiler::~Profiler(void)
        {
            if (m_active)
                end();
        }

        void Profiler::begin(const char* name, const char* file)
        {
            DRK_ASSERT_DEBUG(!m_active, "Profiler is already active");
            m_active = true;
            m_name = name;

            DRK_LOG_CORE_INFO("Beginning Profiler session: {}", m_name);
            m_outStream.open(file);

            auto time = Time::get_local_time();
            double timestamp = get_timestamp(Time::get_global_time());
            write_header(m_name, DRK_VERSION_STRING, std::format("{:%F %T}", time).c_str(), timestamp);
        }

        void Profiler::end(void)
        {
            DRK_ASSERT_DEBUG(m_active, "Profiler is already inactive");
            m_active = false;

            DRK_LOG_CORE_INFO("Ending Profiler session: {}", m_name);
            write_footer();
            m_outStream.close();
        }

        // Could potentially switch to writing Duration Events (ph = B, E) to  get
        // more information about what happened before a crash
        void Profiler::write_profile(const char* cat, const char* name, double start, double duration)
        {
            DRK_ASSERT_DEBUG(m_active, "Profiler is inactive");

            std::stringstream profile;
            profile << std::fixed << std::setprecision(2);
            profile << ",\n";
            profile << "    {";
            profile << "\"cat\":\""  << cat           << "\",";
            profile << "\"name\":\"" << name          << "\",";
            profile << "\"ts\":"     << start         <<   ",";
            profile << "\"dur\":"    << duration      <<   ",";
            profile << "\"ph\":\""   << "X"           << "\",";
            profile << "\"pid\":"    << DRK_GET_PID() <<   ",";
            profile << "\"tid\":"    << std::this_thread::get_id();
            profile << "}";

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
            std::stringstream footer;
            footer << "\n";
            footer << "  ]\n";
            footer << "}\n";

            m_outStream << footer.str();
            m_outStream.flush();
        }

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
    }

#endif
