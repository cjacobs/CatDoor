// ESP32C version

#include <CatDoorController.h>

CatDoorController controller;

void setup()
{
    controller.setup();
}

void loop()
{
    controller.loop();
}
