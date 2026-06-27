#pragma once

struct Config
{
    // Behavior settings
    bool useSleep = true;
    int eventLoopDelay = 100;

    // CatDoor NFC select pins
    int outsideSelectPin = 5;
    int insideSelectPin = 17;

    // SPI pins (shared by both NFC readers)
    int PN532_MOSI = 23;
    int PN532_MISO = 19;
    int PN532_SCK = 18;

    // sensors
    int calibrationButtonPin = 36;
    int hallSensorPin = 16;

    // motor
    int stepPin = 4;
    int dirPin = 0;
    int enablePin = 16;
    int stepsPerRevolution = 48; // 96 for half-steps

    // display / output
    int insideNFCStateLEDPin = 32;
    int outsideNFCStateLEDPin = 33;
    int doorOpenStateLEDPin = 27;
    int speakerPin = 0;

    // I2C (OLED display)
    int I2C_SDA = 21;
    int I2C_SCL = 22;

    // NFC scan timeout (ms)
    int nfcTimeout = 125;
};
