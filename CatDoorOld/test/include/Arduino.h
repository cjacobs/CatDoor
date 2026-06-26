#pragma once
#include <cstdint>
#include <cstring>

using uint8_t = std::uint8_t;
using uint16_t = std::uint16_t;
using uint32_t = std::uint32_t;

#define INPUT_PULLUP 0x02
#define OUTPUT 0x01
#define HIGH 0x1
#define LOW 0x0

inline static uint8_t _fake_pins[64];
inline static unsigned long _fake_millis = 0;

inline void _fake_reset() { memset(_fake_pins, HIGH, sizeof(_fake_pins)); _fake_millis = 0; }
inline void _fake_set_pin(uint8_t pin, uint8_t val) { if (pin < sizeof(_fake_pins)) _fake_pins[pin] = val; }
inline int _fake_get_pin(uint8_t pin) { return (pin < sizeof(_fake_pins)) ? _fake_pins[pin] : HIGH; }

inline void pinMode(uint8_t /*pin*/, int /*mode*/) {}
inline void digitalWrite(uint8_t pin, uint8_t val) { _fake_set_pin(pin, val); }
inline int digitalRead(uint8_t pin) { return _fake_get_pin(pin); }
inline void delay(unsigned long ms) { _fake_millis += ms; }
inline void delayMicroseconds(unsigned int us) { _fake_millis += (us / 1000); }
inline unsigned long millis() { return _fake_millis++; }

// Minimal SPI stub to satisfy includes when not testing PN532
class SPIClass {
public:
  void begin() {}
};

extern SPIClass VSPI;
extern SPIClass HSPI;

// Provide definitions to avoid link errors in the unit-test build
inline SPIClass VSPI;
inline SPIClass HSPI;
