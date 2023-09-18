#include "Timer.hpp"

namespace Morph {

Timer::Timer(TimerResult& result)
    : m_result(&result)
{
    m_start = std::chrono::steady_clock::now();
}

Timer::~Timer()
{
    m_end = std::chrono::steady_clock::now();
    m_result->Set(m_end - m_start);
}

}