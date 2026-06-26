#include <functional>

#include <Arduino.h>

class Timer
{
    public:
    using Value = unsigned long;
    using Callback = std::function<void(Value)>;

    private:
    unsigned long startTime = 0;
    unsigned long interval = 0;
    unsigned long nextFireTime = 0;
    bool running = false;
    Callback callback;

    public:
    Timer(unsigned long interval, Callback&& callback);

    void Start(unsigned long delay=0);
    void Stop();
    void Visit();
};

Timer::Timer(unsigned long interval, Timer::Callback&& callback) : interval(interval), callback(std::move(callback))
{
}

void Timer::Start(unsigned long delay)
{
    running = true;
    startTime = millis();
    nextFireTime = startTime + (delay == 0 ? interval : delay);
}

void Timer::Stop()
{
    running = false;
}

void Timer::Visit()
{
    if (!running)
        return;

    unsigned long currTime = millis();
    // Subtraction-based comparison is rollover-safe: if currTime wraps past
    // nextFireTime, (currTime - nextFireTime) stays a small positive value.
    while ((long)(currTime - nextFireTime) >= 0)
    {
        callback(currTime);
        nextFireTime += interval;
    }
}
