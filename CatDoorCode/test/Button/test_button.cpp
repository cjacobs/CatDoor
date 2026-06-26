#include <Arduino.h>
#include <unity.h>
#include "../include/Arduino.h"
#include "../../src/Button.h"

void setUp() {
  _fake_reset();
}

void test_button_pressed_and_released() {
  Button b(7);
  b.begin();
  _fake_set_pin(7, LOW); // simulate press
  TEST_ASSERT_TRUE(b.pressed());
  _fake_set_pin(7, HIGH);
  TEST_ASSERT_FALSE(b.pressed());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_button_pressed_and_released);
  return UNITY_END();
}
