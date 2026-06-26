#include "DoorController.h"

void DoorController::begin() {
  insideNfcLed.begin();
  outsideNfcLed.begin();
  insideLockLed.begin();
  outsideLockLed.begin();
  hallLed.begin();
  calibButton.begin();
  nfcInside.begin();
  nfcOutside.begin();
  hall.begin();
  lock.begin();
  display.begin();
  state = LockState::Rest;
}

void DoorController::loop() {
  handleNfc();
  updateLeds();
  // idle behavior could be added here
}

void DoorController::handleNfc() {
  auto tagInside = nfcInside.detectTag();
  auto tagOutside = nfcOutside.detectTag();
  if (tagInside != NFCTag::None) {
    performAllowSequence(true);
  } else if (tagOutside != NFCTag::None) {
    performAllowSequence(false);
  }
}

void DoorController::performAllowSequence(bool fromInside) {
  bool allow = true;
  // Check global locked state (simple model: Rest is allowed)
  if (!allow) return;

  if (fromInside) {
    state = LockState::AllowOut;
    lock.allowOut();
  } else {
    state = LockState::AllowIn;
    lock.allowIn();
  }

  opStart = millis();
  // wait for door to open within timeout
  while (millis() - opStart < opTimeout) {
    if (hall.isOpen()) break;
    delay(10);
  }

  // If opened, wait until closed
  if (hall.isOpen()) {
    while (hall.isOpen()) delay(10);
  }

  // return lock to rest
  lock.toRest();
  state = LockState::Rest;
}

void DoorController::updateLeds() {
  insideNfcLed.set(nfcInside.isPresent());
  outsideNfcLed.set(nfcOutside.isPresent());
  insideLockLed.set(state == LockState::AllowOut || state == LockState::Rest);
  outsideLockLed.set(state == LockState::AllowIn || state == LockState::Rest);
  hallLed.set(hall.isOpen());
}
