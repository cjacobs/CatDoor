//
// Simple button / switch class
//

#include <functional>
#include <vector>

class Button {
public:
    using Value = bool;
    using Callback = std::function<void(Value)>;
private:
    int pin;
    uint16_t history = 0;
    unsigned long T1 = 0;
    bool nc = false; // normally-closed
    bool buttonState = false;
    int timeInterval = 0;
    std::vector<Callback> callbacks;

public:
    Button(int pin, bool normally_closed = false, int timeInterval = 5, Callback callback=nullptr)
        : pin(pin)
        , nc(normally_closed)
        , timeInterval(timeInterval)
    {
        addCallback(callback);
    }

    void init()
    {
        pinMode(pin, INPUT_PULLUP);
    }

    void addCallback(const Callback& c)
    {
        if (c)
            callbacks.push_back(c);
    }

    Value getValue()
    {
        auto val = digitalRead(pin);
        if (nc)
            return !val;
        return val;
    }

    void visit();
    
private:
    void invokeCallbacks(Value value)
    {
        for (auto& c : callbacks)
        {
            c(value);
        }
    }

    void updateButtonState(bool newState)
    {
        if (newState != buttonState)
        {
            buttonState = newState;
            invokeCallbacks(buttonState);
        }
    }

    void scan()
    {        
        history <<= 1;
        history |= getValue();
        int sumStates = __builtin_popcount(history);
        if (sumStates >= 10)
            updateButtonState(true);
        else if (sumStates <= 4)
            updateButtonState(false);
    }
};

Button::Callback turnOn(std::function<void(void)> callback)
{
    return [callback](Button::Value val) { if (val) callback(); };
};

Button::Callback turnOff(std::function<void(void)> callback)
{
    return [callback](Button::Value val) { if (!val) callback(); };
};
