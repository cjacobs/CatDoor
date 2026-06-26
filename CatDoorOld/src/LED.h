#pragma once
#include <Arduino.h>

class LED {
public:
  LED(uint8_t pin): pin(pin) {}
  void begin();
  void set(bool on);
private:
  uint8_t pin;
};
