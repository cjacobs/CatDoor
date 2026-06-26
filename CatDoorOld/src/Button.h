#pragma once
#include <Arduino.h>

class Button {
public:
  Button(uint8_t pin): pin(pin) {}
  void begin();
  bool pressed();
private:
  uint8_t pin;
};
