#include <Arduino.h>
#include <unity.h>
#include "../include/Arduino.h"
#include "../../src/LED.h"

void setUp() {
  _fake_reset();
}

void test_led_sets_pin_high_and_low() {
  LED led(3);
  led.begin();
  led.set(true);
  TEST_ASSERT_EQUAL(HIGH, digitalRead(3));
  led.set(false);
  TEST_ASSERT_EQUAL(LOW, digitalRead(3));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_led_sets_pin_high_and_low);
  return UNITY_END();
}
