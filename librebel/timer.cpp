#include "stdincludes.h"
#include "timer.h"

using rebel::Timer;

Timer::Timer()
    : _baseTime(0)
    , _lastFrame(0)
    , _frequency(0)
    , _fps(0.f)
    , _frameTime(0.f)
{ 
    LARGE_INTEGER li = {0};
    QueryPerformanceFrequency(&li);
    _frequency = static_cast<float>(li.QuadPart);
}

long long Timer::getRawTime() const {
    LARGE_INTEGER li = {0};
    QueryPerformanceCounter(&li);
    return li.QuadPart;
}

void Timer::frameTick() {
    long long currentTime = getRawTime();

    if(_lastFrame == 0) {
        _lastFrame = getRawTime();
        _frameTime = 0.f;
        _fps       = 0.f;
    } else {
        _frameTime = (currentTime - _lastFrame) / _frequency;
        _lastFrame = currentTime;
        _fps       = 1.f / _frameTime;
    }
}

float Timer::getAbsoluteTime() {
    return (getRawTime() - _baseTime)  / _frequency;
}

float Timer::getFramesPerSecond() {
    return _fps;
}

float Timer::getFrameTime() {
    return _frameTime;
}

float Timer::getTimeSince(long long prevTime) {
    return (getRawTime() - prevTime) / _frequency;
}

