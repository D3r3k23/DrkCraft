#ifndef DRK_CORE_RESULT_HPP
#define DRK_CORE_RESULT_HPP

namespace DrkCraft
{
    class Result
    {
    public:
        enum class ResultStatus
        {
            Failure = 0,
            Success = 1
        };
        using enum ResultStatus;

    private:
        ResultStatus m_status{Success};

    public:
        Result(const Result&) = default;
        Result& operator=(const Result&) = default;

        Result(ResultStatus status)
            : m_status(status)
        { }

        Result& operator=(ResultStatus status)
        {
            m_status = status;
            return *this;
        }

        operator bool(void) { return m_status != Failure; }

        friend bool operator==(Result r1, Result r2) { return r1.m_status == r2.m_status; }
        friend bool operator!=(Result r1, Result r2) { return !(r1 == r2); }
    };
}

#endif // DRK_CORE_RESULT_HPP
