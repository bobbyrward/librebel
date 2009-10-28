#ifndef LIBREBEL_TIMER_H_
#define LIBREBEL_TIMER_H_

namespace rebel {
    class Timer 
        : public boost::noncopyable
    {
    public:
        Timer();
        
    public:
        void frameTick();

        float getAbsoluteTime();
        float getFramesPerSecond();
        float getFrameTime();
        float getTimeSince(long long prevTime);
        long long getRawTime() const;

    public:
        long long   _baseTime;
        long long   _lastFrame;
        float       _frequency;
        float       _fps;
        float       _frameTime;
    };

    typedef boost::shared_ptr<Timer> TimerPtr;
}

#endif // LIBREBEL_TIMER_H_

