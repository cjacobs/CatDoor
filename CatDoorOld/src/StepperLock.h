#pragma once
#include <Arduino.h>

class StepperLock {
public:
  StepperLock(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin): stepPin(stepPin), dirPin(dirPin), enablePin(enablePin) {}
  void begin();
  void allowIn();
  void allowOut();
  void toRest();
private:
  uint8_t stepPin, dirPin, enablePin;
  void stepTimes(int steps, bool dir);
};
