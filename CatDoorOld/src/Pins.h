#pragma once
#include <Arduino.h>

namespace Pins {
  // LEDs
  inline constexpr uint8_t LED_NFC_INSIDE = 2;
  inline constexpr uint8_t LED_NFC_OUTSIDE = 4;
  inline constexpr uint8_t LED_LOCK_INSIDE = 16;
  inline constexpr uint8_t LED_LOCK_OUTSIDE = 17;
  inline constexpr uint8_t LED_HALL = 5;

  // Buttons
  inline constexpr uint8_t BUTTON_CALIB = 0;

  // NFC CS pins (dummy values)
  inline constexpr uint8_t NFC_CS_INSIDE = 5;
  inline constexpr uint8_t NFC_CS_OUTSIDE = 13;
  inline constexpr int NFC_RESET_PIN = -1;

  // Hall sensor
  inline constexpr uint8_t HALL_PIN = 15;

  // Stepper pins (step, dir, enable)
  inline constexpr uint8_t STEPPER_STEP = 18;
  inline constexpr uint8_t STEPPER_DIR = 19;
  inline constexpr uint8_t STEPPER_ENABLE = 21;

  // I2C (OLED)
  inline constexpr uint8_t I2C_SDA = 21;
  inline constexpr uint8_t I2C_SCL = 22;
}
