#include "Profiler.hpp"

#include <chrono>
#include <iomanip>

namespace DrkCraft
{
    Profiler Profiler::s_instance;

    Profiler& Profiler::get_instance(void)
    {
        return s_instance;
    }

    const char* Profiler::func_name(const char* funcSig)
    {
        return funcSig + 10; // Trim "DrkCraft::"
    }

    Profiler::Profiler(void)
      : m_active(false)
    { }

    void Profiler::begin(const char* file)
    {
        DRK_LOG_INFO("Beginning Profiler session");
        m_outStream.open(file);
        m_outStream << std::setprecision(3) << std::fixed;
        write_header();
        m_active = true;
    }

    void Profiler::end(void)
    {
        if (m_active)
        {
            DRK_LOG_INFO("Ending Profiler session");
            write_footer();
            m_outStream.close();
        }
    }

    void Profiler::write_profile(const char* cat, const char* name, float start, float duration)
    {
        m_outStream << ",\n";
        m_outStream << "    { ";
        m_outStream << "\"cat\":\""  << cat      << "\", ";
        m_outStream << "\"name\":\"" << name     << "\", ";
        m_outStream << "\"ts\":\""   << start    << "\", ";
        m_outStream << "\"dur\":\""  << duration << "\", ";
        m_outStream << "\"ph\":\""   << "X"      << "\", ";
        m_outStream << "\"pid\":"    << 0        <<   ", ";
        m_outStream << "\"tid\":"    << 0;
        m_outStream << " }";

        m_outStream.flush();
    }

    void Profiler::write_header(void)
    {
        m_outStream << "{\n";
        m_outStream <<  "  \"traceEvents\": [\n";
        m_outStream <<  "    { }";

        m_outStream.flush();
    }

    void Profiler::write_footer(void)
    {
        m_outStream << "\n";
        m_outStream << "  ]\n";
        m_outStream << "}\n";

        m_outStream.flush();
    }

    ProfileTimer::ProfileTimer(const char* name, const char* cat)
      : m_name(name),
        m_cat(cat)
    { }

    ProfileTimer::~ProfileTimer(void)
    {
        using std::chrono::duration_cast;
        using std::chrono::microseconds;

        double start   = duration_cast<microseconds>(m_timer.get_start()).count();
        double elapsed = duration_cast<microseconds>(m_timer.get_elapsed()).count();

        Profiler::get_instance().write_profile(m_cat, m_name, start, elapsed);
    }
}
