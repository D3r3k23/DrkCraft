#ifndef DRK_SYSTEM_LOCK_HPP
#define DRK_SYSTEM_LOCK_HPP

#include "Core/Base.hpp"
#include "System/Mutex.hpp"
#include "Core/Debug/Profiler.hpp"

#include <mutex>

namespace DrkCraft
{
    enum LockProfile
    {
        ENABLE_PROFILE,
        DISABLE_PROFILE
    };

    template <LockProfile PROFILE=ENABLE_PROFILE>
    class Lock
    {
        using Lock_t = std::unique_lock<Mutex>;

    public:
        Lock<PROFILE>(Mutex& mutex);

        bool owns_mutex(void) const
        {
            return m_lock.owns_lock();
        }

        constexpr bool profiled(void) const
        {
            return PROFILE == ENABLE_PROFILE;
        }

    private:
        Lock_t m_lock;
    };

    template <LockProfile PROFILE>
    Lock<PROFILE>::Lock(Mutex& mutex)
      : m_lock(mutex)
    { }

    template <>
    Lock<ENABLE_PROFILE>::Lock(Mutex& mutex)
      : m_lock(mutex, std::try_to_lock)
    {
        if (!owns_mutex())
        {
            DRK_PROFILE_LOCK(mutex.name());
            m_lock = Lock_t{mutex};
        }
    }
}

#endif // DRK_SYSTEM_LOCK_HPP
