#include <Arduino.h>

// Simple LED class
//

class LED
{
    int onState;
    int pin;

    public:
    LED(int pin, int onState=HIGH) : pin(pin), onState(onState)
    {
    }

    void init()
    {
        pinMode(pin, OUTPUT);
    }

    void Set(int state)
    {
        digitalWrite(pin, state^onState);
    }
};
