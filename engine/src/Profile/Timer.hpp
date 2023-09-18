#ifndef MORPH_PROFILE_TIMER_HPP
#define MORPH_PROFILE_TIMER_HPP

#include <chrono>
#include <Morph.hpp>

namespace Morph {

class TimerResult
{
private:
    std::chrono::duration<f64> m_duration;
public:
    TimerResult() : m_duration(0) {}
    TimerResult(std::chrono::duration<f64> duration) : m_duration(duration) {}
    inline void Set(std::chrono::duration<f64> duration) { m_duration = duration; }
    inline f64 GetSeconds() { return m_duration.count(); }
    inline f64 GetMiliSeconds() { return std::chrono::duration<f64, std::ratio<1, 1000>>(m_duration).count(); }
};

class Timer
{
private:
    TimerResult* m_result;
    std::chrono::time_point<std::chrono::steady_clock> m_start;
    std::chrono::time_point<std::chrono::steady_clock> m_end;
public:
    Timer(TimerResult& result);

    ~Timer();
};

}

#endif // MORPH_PROFILE_TIMER_HPP