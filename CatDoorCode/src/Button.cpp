#include "Button.h"

void Button::begin() {
  pinMode(pin, INPUT_PULLUP);
}

bool Button::pressed() {
  return digitalRead(pin) == LOW;
}
