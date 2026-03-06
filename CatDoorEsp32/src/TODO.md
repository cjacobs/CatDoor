TODO


## Basic inventory of classes, etc.

### Controls:
  - queryable state
  - callbacks for state-change events

  - pushbuttons
  - switches (same thing as buttons?)
  - hall-effect sensors (acts as a button/switc?)
  - NFC readers
  - motion sensors

### Actuators:
  - callbacks? -- can they operate asynchronously and report when an action is done?
  - stepper motor

### "Display" objects:
  - LED
  - OLED display
  - speaker

### "Host" / application
The host acts as the basic entry point and event loop for the application. 
During each pass through the event loop, it services the devices, giving each a time slice.

It also services the peripherals and communicates with the state machine(s):
  - feeds events (from devices, timers, ...) to state machine
  - gets callbacks from state machine to control devices


### State machine
The state machine


## Basic state machine architecture


