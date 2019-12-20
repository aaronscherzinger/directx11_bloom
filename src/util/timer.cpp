#include "util/timer.h"

#include <assert.h>

Timer::Timer() noexcept
: m_elapsedTimeMilliseconds(0.f)
, m_isRunning(false)
{
}

void Timer::Start() noexcept
{
    assert(!m_isRunning);

    m_startTime = std::chrono::high_resolution_clock::now();
    m_isRunning = true;
}

void Timer::Stop() noexcept
{
    assert(m_isRunning);

    m_endTime = std::chrono::high_resolution_clock::now();
    m_isRunning = false;
    m_elapsedTimeMilliseconds = std::chrono::duration<float, std::chrono::milliseconds::period>(m_endTime - m_startTime).count();
}

float Timer::GetElapsedTimeMilliseconds() const noexcept
{
    assert(!m_isRunning);

    return m_elapsedTimeMilliseconds;
}
