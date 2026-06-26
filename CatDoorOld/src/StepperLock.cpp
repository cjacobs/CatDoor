#include "StepperLock.h"

void StepperLock::begin() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
}

void StepperLock::stepTimes(int steps, bool dir) {
  digitalWrite(dirPin, dir ? HIGH : LOW);
  for (int i = 0; i < steps; ++i) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
  }
}

void StepperLock::allowIn() {
  stepTimes(200, false);
}

void StepperLock::allowOut() {
  stepTimes(200, true);
}

void StepperLock::toRest() {
  stepTimes(100, false);
}
