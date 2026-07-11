#pragma once

#include <Arduino.h>

// Simple LED class
//

class LED
{
    int onState;
    int pin;

  public:
    LED(int pin, int onState = HIGH) : pin(pin), onState(onState)
    {
    }

    void init()
    {
        pinMode(pin, OUTPUT);
    }

    int getState()
    {
        return digitalRead(pin) ^ onState;
    }

    void Set(int state)
    {
        digitalWrite(pin, state ^ onState);
    }
};
