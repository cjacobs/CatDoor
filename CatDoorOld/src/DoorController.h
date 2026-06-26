#pragma once
#include <Arduino.h>
#include "LED.h"
#include "Button.h"
#include "NFCReader.h"
#include "HallSensor.h"
#include "StepperLock.h"
#include "Display.h"
#include "Pins.h"

enum class LockState { Rest, AllowIn, AllowOut, Locked };

class DoorController {
public:
  void begin();
  void loop();
private:
  LED insideNfcLed{Pins::LED_NFC_INSIDE};
  LED outsideNfcLed{Pins::LED_NFC_OUTSIDE};
  LED insideLockLed{Pins::LED_LOCK_INSIDE};
  LED outsideLockLed{Pins::LED_LOCK_OUTSIDE};
  LED hallLed{Pins::LED_HALL};

  Button calibButton{Pins::BUTTON_CALIB};
  NFCReader nfcInside{&vspi, Pins::NFC_CS_INSIDE, Pins::NFC_RESET_PIN};
  NFCReader nfcOutside{&hspi, Pins::NFC_CS_OUTSIDE, Pins::NFC_RESET_PIN};
  HallSensor hall{Pins::HALL_PIN};
  StepperLock lock{Pins::STEPPER_STEP, Pins::STEPPER_DIR, Pins::STEPPER_ENABLE};
  Display display;

  LockState state = LockState::Rest;
  unsigned long opStart = 0;
  const unsigned long opTimeout = 10000;

  void handleNfc();
  void updateLeds();
  void performAllowSequence(bool fromInside);
};
