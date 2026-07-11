#pragma once

//
// Simple button / switch class
//

#include <Arduino.h>
#include <functional>
#include <vector>

class Button
{
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

    static volatile bool gotInterrupt;

    static void interrupt()
    {
        gotInterrupt = true;
    }

  public:
    Button(int pin, bool normally_closed = false, int timeInterval = 5, Callback callback = nullptr)
        : pin(pin), nc(normally_closed), timeInterval(timeInterval)
    {
        addCallback(callback);
    }

    void init()
    {
        pinMode(pin, INPUT_PULLUP);

        // attach interrupts
        Serial.print("Interrupt pin: ");
        Serial.println(digitalPinToInterrupt(pin));
        attachInterrupt(digitalPinToInterrupt(pin), interrupt, CHANGE);
    }

    void addCallback(const Callback &c)
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

    void visit()
    {
        if (gotInterrupt)
            scan();
        gotInterrupt = false; // TODO: fix this -- it's global
    }

  private:
    void invokeCallbacks(Value value)
    {
        for (auto &c : callbacks)
        {
            c(value);
        }
    }

    void updateButtonState(bool newState)
    {
        if (newState != buttonState)
        {
            Serial.print("update button state, old: ");
            Serial.print(buttonState);
            Serial.print(" new: ");
            Serial.println(newState);

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

inline Button::Callback turnOn(std::function<void(void)> callback)
{
    return [callback](Button::Value val) {
        if (val)
            callback();
    };
};

inline Button::Callback turnOff(std::function<void(void)> callback)
{
    return [callback](Button::Value val) {
        if (!val)
            callback();
    };
};
