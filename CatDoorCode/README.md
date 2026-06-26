# CatDoor project

CatDoor is a project that controls a "smart" cat door. It can set the current state of the door (open, locked, locked for coming in, locked for going out), and can override that setting if it senses a special cat tag via an NFC reader.


## Existing hardware prototype
The existing cat door prototype is based on an ESP32 DevKit C board. It has the following sensors:

- 2 PN532-based NFC sensor boards (one inside the door and one outside), connected via SPI
- a hall-effect sensor to detect when the door is closed
- 1 click-on/click-off button to engage "calibration" mode

The prototype also has the following display/output devices:
- a 128x64 monochrome SSD1306-based OLED display, connected via I2C
- 2 LEDs that reflect the state of the NFC readers (the "inside" LED lights up when the inside NFC reader detects an NFC tag, and likewise for the "outside" LED)
- 2 green LEDs (inside & outside) that reflect the state of the door lock (the LED is lit if the lock allows movement from the corresponding direction. i.e., the "inside" light is on if the door lock state allows going through the door from the inside to the outside)
- 1 LED that reports what the hall-effect sensor is reading (that is, if the door is closed or not)

The door lock mechanism consists of a stepper motor and a wheel that has 2 half-discs rotated 90 degrees from each other. There is one disc on each side of the door, and blocks the door when its "existing" half is up, and allows the door to open when the missing half is up.

## Software version 1
The behavior of the cat door is as follows:
- LEDs should reflect current state of the system
- if an NFC reader senses an NFC tag (from a cat's collar), the following should happen:
  - the lock mechanism should turn to the appropriate setting to allow the cat to move through the door, if the current state allows it.
  - it should wait for some amount of time for the cat to go through the door (using the hall-effect sensor to detect if the door has opened)
  - if the door does open before the operation times out, then wait until the door is closed and then the lock back to its resting state, otherwise just move the lock back to its resting state


As for the code, it should use the Arduino platform, but instead of using the Arduino IDE, it should be a PlatformIO project. It should have the following features:

- Clean design using C++. C++ 20 is available to use.
- Simple classes for hardware objects (LEDs, buttons, etc.)
- Break the implementation info separate files for each class / concept


## Software version 2
After the above is implemented, there should be a web interface that allows the user to set the lock state and allow setting a schedule for when the door should be locked in which direction. The ESP32 controller contains a WiFi interface.

