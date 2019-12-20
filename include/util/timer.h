#pragma once

#include <chrono>

class Timer
{
public:
    Timer() noexcept;

    // no copy or move operations allowed
    Timer(const Timer& other) = delete;
    Timer(Timer&& other) = delete;
    Timer& operator=(const Timer& other) = delete;
    Timer& operator=(Timer&& other) = delete;

    void Start() noexcept;
    void Stop() noexcept;
    float GetElapsedTimeMilliseconds() const noexcept;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;
    float m_elapsedTimeMilliseconds;
    bool m_isRunning;
};
