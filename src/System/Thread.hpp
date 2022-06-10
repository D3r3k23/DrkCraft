#ifndef DRK_SYSTEM_THREAD_HPP
#define DRK_SYSTEM_THREAD_HPP

#include "Core/Base.hpp"
#include "Core/Debug/Profiler.hpp"

#include <thread>
#include <optional>
#include <functional>
#include <concepts>
#include <utility>

namespace DrkCraft
{
    using ThreadType = std::jthread;
    using StopToken = std::stop_token;

    template <typename ST=NoneType>
    class Thread
    {
    public:
        using StopCallbackFn = std::function<void(void)>;
        using StopCallback = std::stop_callback<StopCallbackFn>;

        Thread(void) = default;

        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

        Thread(const char* name, auto fn)
        {
            if constexpr (std::same_as<ST, StopToken>)
                start<StopToken>(name, std::move(fn));
            else
                start(name, std::move(fn));
        }

        template <typename ... Args>
        Thread(const char* name, auto fn, Args&& ... args)
        {
            start(name, std::move(fn), std::forward<Args>(args)...);
        }

        template <typename ... Args>
        Thread(const char* name, auto fn, StopCallbackFn cb, Args&& ... args)
          : Thread(name, std::move(fn), std::forward<Args>(args)...)
        {
            add_stop_callback(std::move(cb));
        }

        Thread(Thread&& other)
          : m_handle(std::move(other.m_handle)),
            m_stopToken(m_handle.get_stop_token())
        {
            if (other.has_stop_callback())
                add_stop_callback(std::move(other.m_stopCallbackFn));

            other.m_stopToken.reset();
            other.m_stopCallback.reset();
        }

        Thread& operator=(Thread&& other)
        {
            m_handle = std::move(other.m_handle);
            m_stopToken = m_handle.get_stop_token();

            if (other.has_stop_callback())
                add_stop_callback(std::move(other.m_stopCallbackFn));

            other.m_stopToken.reset();
            other.m_stopCallback.reset();

            return *this;
        }

        void start(const char* name, auto fn)
        {
            DRK_ASSERT_DEBUG(!joinable(), "Thread is joinable!");

            DRK_PROFILE_THREAD_CREATE(name);
            m_handle = ThreadType([name, fn=std::move(fn)]
            {
                DRK_PROFILE_THREAD(name);
                fn();
            });
            m_stopToken = m_handle.get_stop_token();
        }

        template <typename ST>
        void start(const char* name, auto fn)
        {
            DRK_ASSERT_DEBUG(!joinable(), "Thread is joinable!");

            DRK_PROFILE_THREAD_CREATE(name);
            m_handle = ThreadType([name, fn=std::move(fn)](ST st)
            {
                DRK_PROFILE_THREAD(name);
                fn(st);
            });
            m_stopToken = m_handle.get_stop_token();
        }

        template <typename ... Args>
        void start(const char* name, auto fn, Args&& ... args)
        {
            DRK_ASSERT_DEBUG(!joinable(), "Thread is joinable!");

            DRK_PROFILE_THREAD_CREATE(name);
            m_handle = ThreadType([name, fn=std::move(fn)](auto&& ... args)
            {
                DRK_PROFILE_THREAD(name);
                fn(std::forward<decltype(args)>(args)...);
            }, std::forward<Args>(args)...);

            m_stopToken = m_handle.get_stop_token();
        }

        void add_stop_callback(StopCallbackFn cb)
        {
            m_stopCallbackFn = std::move(cb);
            m_stopCallback.emplace(*m_stopToken, m_stopCallbackFn);
        }

        bool has_stop_callback(void) const
        {
            return (bool)m_stopCallbackFn;
        }

        void stop(void)
        {
            DRK_ASSERT_DEBUG(joinable(), "Thread is not joinable!");
            m_handle.request_stop();
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
        };

    private:
        ThreadType m_handle;
        std::optional<StopToken> m_stopToken;
        std::optional<StopCallback> m_stopCallback;
        StopCallbackFn m_stopCallbackFn;
    };
}

#endif // DRK_SYSTEM_THREAD_HPP
