#ifndef DRK_SYSTEM_THREAD_HPP
#define DRK_SYSTEM_THREAD_HPP

#include "Core/Base.hpp"
#include "Util/Fn.hpp"
#include "Core/Debug/Profiler.hpp"

#include <thread>
#include <functional>
#include <concepts>
#include <utility>

namespace DrkCraft
{
    using ThreadType = std::jthread;
    using StopToken = std::stop_token;

    template <typename ST=None>
    class Thread
    {
    public:
        using StopCallbackFn = std::function<void(void)>;

        Thread(void) = default;

        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

        Thread(const char* name, auto fn)
          : m_name(name)
        {
            if constexpr (std::same_as<ST, StopToken>)
                start<StopToken>(std::move(fn));
            else
                start(std::move(fn));
        }

        template <typename ... Args>
        Thread(const char* name, auto fn, Args&& ... args)
          : m_name(name)
        {
            start(std::move(fn), std::forward<Args>(args)...);
        }

        template <typename ... Args>
        Thread(const char* name, auto fn, StopCallbackFn&& cb, Args&& ... args)
          : Thread(name, std::move(fn), std::forward<Args>(args)...)
        {
            add_stop_callback(std::forward<StopCallbackFn>(cb));
        }

        Thread(Thread&& other)
          : m_name(other.m_name),
            m_handle(std::move(other.m_handle)),
            m_stopCallbackFn(std::move(other.m_stopCallbackFn))
        {
            other = {};
        }

        Thread& operator=(Thread&& other)
        {
            m_name = other.m_name;
            m_handle = std::move(other.m_handle);
            m_stopCallbackFn = std::move(other.m_stopCallbackFn);

            other = {};

            return *this;
        }

        void start(auto fn)
        {
            DRK_ASSERT_DEBUG(!joinable(), "Thread is joinable!");
            DRK_LOG_CORE_INFO("Creating thread: {}", m_name);
            m_stopped = false;

            DRK_PROFILE_THREAD_CREATE(m_name);
            m_handle = ThreadType([this, fn=std::move(fn)]
            {
                DRK_PROFILE_THREAD(m_name);
                fn();
                m_stopCallbackFn();
            });
        }

        template <typename ST>
        void start(auto fn)
        {
            DRK_ASSERT_DEBUG(!joinable(), "Thread is joinable!");
            DRK_LOG_CORE_INFO("Creating thread: {}", m_name);
            m_stopped = false;

            DRK_PROFILE_THREAD_CREATE(m_name);
            m_handle = ThreadType([this, fn=std::move(fn)](ST st)
            {
                DRK_PROFILE_THREAD(m_name);
                fn(st);
                m_stopCallbackFn();
            });
        }

        template <typename ... Args>
        void start(auto fn, Args&& ... args)
        {
            DRK_ASSERT_DEBUG(!joinable(), "Thread is joinable!");
            DRK_LOG_CORE_INFO("Creating thread: {}", m_name);
            m_stopped = false;

            DRK_PROFILE_THREAD_CREATE(m_name);
            m_handle = ThreadType([this, fn=std::move(fn)](auto&& ... args)
            {
                DRK_PROFILE_THREAD(m_name);
                fn(std::forward<decltype(args)>(args)...);
                m_stopCallbackFn();
            }, std::forward<Args>(args)...);
        }

        void add_stop_callback(StopCallbackFn cb)
        {
            StopCallbackFn currentCallback = std::move(m_stopCallbackFn);
            m_stopCallbackFn = add_prefix(std::move(cb), std::move(currentCallback));
        }

        void stop(void)
        {
            DRK_ASSERT_DEBUG(joinable(), "Thread is not joinable!");
            m_handle.request_stop();
        }

        bool stopped(void) const
        {
            return m_stopped;
        }

        void join(void)
        {
            DRK_ASSERT_DEBUG(joinable(), "Thread is not joinable!");
            m_handle.join();
        }

        bool joinable(void) const
        {
            return m_handle.joinable();
        }

        operator bool(void) const
        {
            return joinable();
        }

        static uint count(void)
        {
            return ThreadType::hardware_concurrency();
        }

        struct This
        {
            static auto id(void)
            {
                return std::this_thread::get_id();
            }

            static void sleep_for(const auto& duration)
            {
                std::this_thread::sleep_for(duration);
            }

            static void sleep_until(const auto& time)
            {
                std::this_thread::sleep_until(time);
            }
        };

    private:
        inline constexpr StopCallbackFn s_basic_stop_callback = [this]()
        {
            m_stopped = true;
        };

        const char* m_name = "";
        ThreadType m_handle;
        StopCallbackFn m_stopCallbackFn{s_basic_stop_callback};
        bool m_stopped = false;
    };
}

#endif // DRK_SYSTEM_THREAD_HPP
