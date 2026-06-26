#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

enum class NFCTag { None, Present };

class NFCReader {
public:
  NFCReader(SPIClass* spi, int csPin, int resetPin = -1);
  void begin();
  NFCTag detectTag(uint16_t timeout = 50);
  bool isPresent(uint16_t timeout = 10);
private:
  SPIClass* spi;
  int csPin;
  int resetPin;
  Adafruit_PN532* pn532 = nullptr;
};

extern SPIClass hspi;
extern SPIClass vspi;

