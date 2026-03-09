//
// "business logic" for the cat door
//

// TODO: abstract out all the Arduino-specific stuff

#include <SPI.h>
#include <Wire.h>

#include <Button.h>
#include <HAL.h>
#include <LED.h>
#include <NFCReader.h>
#include <StateMachine.h>
#include <Stepper.h>

namespace Controller {
// CatDoor pin definitions

const int insideNFCChipSelectPin = 5;
const int outsideNFCChipSelectPin = 17;

// TODO: pass in pins to NFC obj?
const int PN532_MOSI = 23;
const int PN532_MISO = 19;
const int PN532_SCK = 18;

// sensors
const int calibrationButtonPin = 36;
const int hallSensorPin = 39;

// display / output
const int insideNFCStateLEDPin = 32;
const int outsideNFCStateLEDPin = 33;
const int doorOpenStateLEDPin = 27;

const int speakerPin = 25;

// motor
const int stepPin = 4;
const int dirPin = 0;
const int enablePin = 15;

//
// Use this line for a breakout with a software SPI connection (recommended):
NFCReader insideNFC(insideNFCChipSelectPin);
NFCReader outsideNFC(outsideNFCChipSelectPin);

const int nfcTimeout = 125;

LED insideNFCStateLED(insideNFCStateLEDPin);
LED outsideNFCStateLED(outsideNFCStateLEDPin);
LED doorOpenStateLED(doorOpenStateLEDPin);

// speaker
const bool useHalfSteps = false;
const int stepsPerRevolution = useHalfSteps ? 96 : 48;
Stepper lockStepper(stepPin, dirPin, enablePin, stepsPerRevolution);

Button calibrationButton(calibrationButtonPin, true);
Button hallSensorButton(hallSensorPin, true);

// OLED display
const int I2C_SDA = 21;
const int I2C_SCL = 22;
TwoWire I2C1 = TwoWire(0); // I2C1 bus

void printIfElse(int val, const char* label, const char* trueLabel, const char* falseLabel)
{
    if (!Serial)
        return;

    Serial.print(label);
    if (val)
        Serial.print(trueLabel);
    else
        Serial.print(falseLabel);
}

void waitForKeypress()
{
    if (!Serial)
        return;

    // Wait a bit before trying again
    Serial.println("\n\nSend a character to scan another tag!!!!");
    Serial.flush();
    while (!Serial.available())
        ;
    while (Serial.available()) {
        Serial.read();
    }
    Serial.flush();
}

void calibrate()
{
    // ...
    Serial.println("Calibrating...");
}

void visit(NFCReader& nfc, LED& led)
{
    uint8_t uid[8]; 
    uint8_t uidLength;
    if (nfc.valid()) {
        if (nfc.readTag(uid, &uidLength, nfcTimeout)) {
            if (Serial)
                Serial.println("INSIDE!");
            // processTag(uid, uidLength);
            // led.SetLED(HIGH);
        } else {
            // led.SetLED(LOW);
        }
    }
}

//
//
//
void setup()
{
    auto startTime = millis();
    Serial.begin(115200);
    // while (!Serial && ((millis() - startTime) < 1000))
    while (!Serial)
        delay(10); // for Leonardo/Micro/Zero

    if (Serial)
        Serial.println("Hello!");

    calibrationButton.init();
    calibrationButton.addCallback(turnOn(calibrate));

    lockStepper.init();

    if (Serial)
        Serial.println("Initializing inside NFC reader");
    insideNFC.begin();
    if (!insideNFC.valid())
        if (Serial)
            Serial.println("Error initializing inside NFC reader");

    if (Serial)
        Serial.println("Initializing outside NFC reader");
    outsideNFC.begin();
    if (!outsideNFC.valid())
        if (Serial)
            Serial.println("Error initializing inside NFC reader");
}

void scanInputs()
{
    // TODO: check NFC IRQ states?

    calibrationButton.visit();
    hallSensorButton.visit();

    visit(insideNFC, insideNFCStateLED);
    visit(outsideNFC, outsideNFCStateLED);
}

void printOutputs()
{
    doorOpenStateLED.Set(hallSensorButton.getValue());

    // move this stuff to scanInputs?
    // int calibration = calibrationButton.getValue();

    // printIfElse(calibration, "cal: ", "X", " ");
    // printIfElse(insideLock, "\tinside lock: ", "  locked", "unlocked");
    // printIfElse(outsideLock, "\toutside lock: ", "  locked", "unlocked");
    // printIfElse(insideMotion, "\t", "inside motion", "             ");
    // printIfElse(outsideMotion, "\t", "outside motion", "              ");
    if (Serial) {
        Serial.print("\thall:");
        Serial.print(hallSensorButton.getValue());

        Serial.println("");
    }
}

void loop()
{
    scanInputs();

    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
    uint8_t uidLength; // Length of the UID (4 or 7 bytes depending on ISO14443A
    // card type)

    // auto calibrate = calibrationButton.buttonPressed();
    auto calibrate = calibrationButton.getValue();
    if (calibrate) {
        // For now, just sets zero pos
        lockStepper.reset();
    } else {
    }

    delay(20);
}

//
// TODO: eventually migrate everything here
//

struct Event { };

class CatDoorController {
    // TODO: add state machine

public:
    CatDoorController() { }

    void feed(const Event& event);
};

}
