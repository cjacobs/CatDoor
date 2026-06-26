TODO

- abstract out all the Arduino-specific stuff
- check NFC IRQ states
- read hall-effect state
- use sleep mode and wake on interrupt
- check for correct NFC tag?
- roll over time value at some point
- implement state machine and use it
- make a config class that contains all the pin assignments and active high/low, plus other configuration params, such as
  - loop rate
  - enable sleep mode
  - web stuff
  - ...
- eventually migrate everything from Controller namespace into CatDoorController class
