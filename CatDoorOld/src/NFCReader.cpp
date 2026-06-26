#include "NFCReader.h"

NFCReader::NFCReader(SPIClass* spi, int csPin, int resetPin)
  : spi(spi), csPin(csPin), resetPin(resetPin) {}

void NFCReader::begin() {
  if (!pn532) {
    pn532 = new Adafruit_PN532(csPin);
  }
  // initialize SPI bus (ESP32 has VSPI/HSPI instances)
  if (spi) spi->begin();
  pn532->begin();
  uint32_t versiondata = pn532->getFirmwareVersion();
  if (versiondata) {
    pn532->SAMConfig();
  }
}

NFCTag NFCReader::detectTag(uint16_t timeout) {
  uint8_t uid[7];
  uint8_t uidLen = 0;
  if (pn532->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLen, timeout)) {
    return NFCTag::Present;
  }
  return NFCTag::None;
}

bool NFCReader::isPresent(uint16_t timeout) {
  return detectTag(timeout) == NFCTag::Present;
}

bool NFCReader::isPresent() {
  return present;
}
