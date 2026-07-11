#include "Log.h"

#include <Arduino.h>

void Log(char *message)
{
    if (Serial)
        Serial.print(message);
}
