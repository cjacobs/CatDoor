#include <Arduino.h>
#include "DoorController.h"

DoorController controller;

void setup() {
  Serial.begin(115200);

    while (!Serial)
        delay(10); // for Leonardo/Micro/Zero

    if (Serial)
        Serial.println("Hello!");

  controller.begin();
}

void loop() {
  controller.loop();
}
