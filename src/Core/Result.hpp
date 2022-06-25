#ifndef DRK_CORE_RESULT_HPP
#define DRK_CORE_RESULT_HPP

#include "Core/Type.hpp"

#include "lib/string.hpp"
#include "lib/string_view.hpp"

#include <concepts>
#include <utility>
#include <memory>

namespace DrkCraft
{
    enum class ResultStatus
    {
        Failure = 0,
        Success = 1
    };

    template <typename T=None>
    class Result
    {
    public:
        using Status = ResultStatus; // Result::Status
        using enum ResultStatus; // Result::Failure, Result::Success

    public:
        Result(void)
          : m_value{}
        { }

        Result(auto&& ... args)
          : m_value(std::forward<decltype(args)>(args)...)
        { }

        Result(const Result&) = default;
        Result(Result&&) = default;
        Result& operator=(const Result&) = default;
        Result& operator=(Result&&) = default;

        Result(ResultStatus status)
        {
            if (status == ResultStatus::Success)
                m_value = T{};
        }

        Result& operator=(ResultStatus status)
        {
            std::swap(*this, Result<T>(status));
            return *this;
        }

        const T& get_value(void) const
        {
            return m_value.value();
        }

        const T& operator*(void) const
        {
            return *m_value;
        }

        const T* operator->(void) const
        {
            return m_value.operator->();;
        }

        bool has_value(void) const
        {
            return m_value.has_value();
        }

        bool has_error(void) const
        {
            return !has_value();
        }

        operator bool(void) const
        {
            return !has_error();
        }

        ResultStatus status(void) const
        {
            return has_error() ? ResultStatus::Failure : ResultStatus::Success;
        }

        bool operator==(ResultStatus s) const
        {
            return status() == s;
        }

    private:
        Optional<T> m_value;
    };

    template <typename E=None>
    class Error : public Result<E>
    {
    public:
        Error(void)
          : Result<E>()
        { }

        Error(auto&& ... args)
          : Result<E>(std::forward<decltype(args)>(args)...)
        { }

        Error(const Error&) = default;
        Error(Error&&) = default;
        Error& operator=(const Error&) = default;
        Error& operator=(Error&&) = default;

        Error(ResultStatus status)
          : Result<E>(status)
        { }

        bool has_error(void) const
        {
            return this->has_value();
        }

        const E& get_error(void) const
        {
            return this->get_value();
        }
    };

    class ErrorMsg : public Error<string>
    {
    public:
        using Error<string>::Error;

        string_view get_msg(void) const
        {
            return m_msg;
        }

    private:
        string m_msg;
    };
}

#endif // DRK_CORE_RESULT_HPP
