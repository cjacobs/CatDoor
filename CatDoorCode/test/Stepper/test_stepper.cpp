#include <Arduino.h>
#include <unity.h>
#include "../include/Arduino.h"
#include "../../src/StepperLock.h"

void setUp() {
  _fake_reset();
}

void test_stepper_sets_dir_and_steps() {
  StepperLock s(10, 11, 12);
  s.begin();
  TEST_ASSERT_EQUAL(LOW, digitalRead(12)); // enable set LOW in begin()
  s.allowOut();
  TEST_ASSERT_EQUAL(HIGH, digitalRead(11)); // dir true for allowOut
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_stepper_sets_dir_and_steps);
  return UNITY_END();
}
