//
// This file just forwards the Arduino setup() and loop() functions to the controller
//

#include <CatDoorController.h>

void setup()
{
    Controller::setup();
}

void loop()
{
    Controller::loop();
}
