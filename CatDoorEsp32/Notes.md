

```
Building in release mode
Retrieving maximum program size .pio/build/esp32-DevKitC/firmware.elf
Checking size .pio/build/esp32-DevKitC/firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [=         ]   6.8% (used 22120 bytes from 327680 bytes)
Flash: [==        ]  22.8% (used 298773 bytes from 1310720 bytes)
Configuring upload protocol...
AVAILABLE: cmsis-dap, esp-bridge, esp-prog, espota, esptool, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa
CURRENT: upload_protocol = esptool
Looking for upload port...
Auto-detected: /dev/cu.usbserial-0001
Uploading .pio/build/esp32-DevKitC/firmware.bin
esptool.py v4.9.0
Serial port /dev/cu.usbserial-0001
Connecting..........
Chip is ESP32-D0WD-V3 (revision v3.1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz
MAC: a4:f0:0f:77:d5:98
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
WARNING: Failed to communicate with the flash chip, read/write operations will fail. Try checking the chip connections or removing any other hardware connected to IOs.
Configuring flash size...
Flash will be erased from 0x00001000 to 0x00005fff...
Flash will be erased from 0x00008000 to 0x00008fff...
Flash will be erased from 0x0000e000 to 0x0000ffff...
Flash will be erased from 0x00010000 to 0x00059fff...
SHA digest in image updated
Compressed 17536 bytes to 12202...

A fatal error occurred: Packet content transfer stopped (received 8 bytes)
*** [upload] Error 2
========================================
```