#include <functional>

#include <Arduino.h>

class Timer 
{
    public:
    using Value = int;
    using Callback = std::function<void(Value)>;
    
    private:
    int startTime = 0;
    int interval = 0;
    int nextFireTime = 0;
    bool running = false;
    Callback callback;

    public:
    Timer(int interval, Callback&& callback);

    void Start(int delay=0);
    void Stop();
    void Visit();
};

Timer::Timer(int interval, Timer::Callback&& callback) : interval(interval), callback(std::move(callback))
{
}

void Timer::Start(int delay)
{
    running = true;
    startTime = millis();
    nextFireTime = startTime + delay == 0 ? interval : delay;
}

void Timer::Stop()
{
    running = false;

}

void Timer::Visit()
{
    auto currTime = millis();
    while (currTime >= nextFireTime)
    {
        callback(currTime);
        nextFireTime += interval; 
    }

    // TODO: rollover at some point
}