#include <Arduino.h>
#include <unity.h>
#include "../include/Arduino.h"
#include "../../src/HallSensor.h"

void setUp() {
  _fake_reset();
}

void test_hall_reports_open_and_closed() {
  HallSensor h(9);
  h.begin();
  _fake_set_pin(9, LOW); // open
  TEST_ASSERT_TRUE(h.isOpen());
  _fake_set_pin(9, HIGH);
  TEST_ASSERT_FALSE(h.isOpen());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_hall_reports_open_and_closed);
  return UNITY_END();
}
