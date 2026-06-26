#include "LED.h"

void LED::begin() {
  pinMode(pin, OUTPUT);
  set(false);
}

void LED::set(bool on) {
  digitalWrite(pin, on ? HIGH : LOW);
}
