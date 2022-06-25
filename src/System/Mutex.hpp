#ifndef DRK_SYSTEM_MUTEX_HPP
#define DRK_SYSTEM_MUTEX_HPP

#include "Core/Base.hpp"

#include <mutex>

namespace DrkCraft
{
    class Mutex
    {
        using Mutex_t = std::mutex;

    public:
        Mutex(const char* name="mutex")
          : m_mutex(),
            m_name(name)
        { }

        void lock(void)
        {
            m_mutex.lock();
        }

        bool try_lock(void)
        {
            return m_mutex.try_lock();
        }

        void unlock(void)
        {
            m_mutex.unlock();
        }

        const char* name(void) const
        {
            return m_name;
        }

    private:
        Mutex_t m_mutex;
        const char* m_name;
    };
}

#endif // DRK_SYSTEM_MUTEX_HPP
