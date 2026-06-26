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
        Serial.print("initing LED on pin ");
        Serial.println(pin);
        pinMode(pin, OUTPUT);
    }

    int getState() 
    {
        Serial.print("Value for LED on pin ");
        Serial.print(pin);
        Serial.print(": ");
        Serial.println(digitalRead(pin));
        return digitalRead(pin);
    }
    
    void Set(int state)
    {
        Serial.print("Setting LED on pin ");
        Serial.print(pin);
        Serial.print(" to ");
        Serial.println(state^onState);
        digitalWrite(pin, state^onState);
    }
};
