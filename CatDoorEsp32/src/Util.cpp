#include <Arduino.h>

#include <Util.h>

void printIfElse(int val, const char* label, const char* trueLabel, const char* falseLabel)
{
    if (!Serial)
        return;

    Serial.print(label);
    if (val)
        Serial.print(trueLabel);
    else
        Serial.print(falseLabel);
}

void waitForKeypress()
{
    if (!Serial)
        return;

    // Wait a bit before trying again
    Serial.println("\n\nSend a character to scan another tag!!!!");
    Serial.flush();
    while (!Serial.available())
        ;
    while (Serial.available()) {
        Serial.read();
    }
    Serial.flush();
}

