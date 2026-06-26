#include <Arduino.h>
#include "DoorController.h"

DoorController controller;

void setup() {
  Serial.begin(115200);
  controller.begin();
}

void loop() {
  controller.loop();
  delay(10);
}
