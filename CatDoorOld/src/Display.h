#pragma once
#include <Arduino.h>

class Display {
public:
  void begin();
  void showStatus(const char* s);
};
