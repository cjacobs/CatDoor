#pragma once
#include <Arduino.h>

class HallSensor {
public:
  HallSensor(uint8_t pin): pin(pin) {}
  void begin();
  bool isOpen();
private:
  uint8_t pin;
};
