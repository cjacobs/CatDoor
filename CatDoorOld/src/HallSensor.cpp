#include "HallSensor.h"

void HallSensor::begin() {
  pinMode(pin, INPUT_PULLUP);
}

bool HallSensor::isOpen() {
  return digitalRead(pin) == LOW;
}
