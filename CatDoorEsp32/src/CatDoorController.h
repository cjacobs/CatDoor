//
// "business logic" for the cat door
//

// TODO: abstract out all the Arduino-specific stuff

#include <SPI.h>
#include <Wire.h>

#include <HAL.h>
#include <StateMachine.h>
#include <Button.h>
#include <LED.h>
#include <NFCReader.h>
#include <Stepper.h>

namespace Controller
{
const int outsideSelectPin = 5;
const int insideSelectPin = 17;

// TODO: pass in pins to NFC obj?
const int PN532_MOSI = 23;
const int PN532_MISO = 19;
const int PN532_SCK = 18;


// CatDoor pins

// sensors
const int calibrationButtonPin = 36;
const int hallSensorPin = 16;

// motor
const int stepPin = 4;
const int dirPin = 0;
const int enablePin = 16;

// display / output
const int insideNFCStateLEDPin = 32;
const int outsideNFCStateLEDPin = 33;
const int doorOpenStateLEDPin = 27;

const int speakerPin = 0;

//
// Use this line for a breakout with a software SPI connection (recommended):
NFCReader insideNFC(insideSelectPin);
NFCReader outsideNFC(outsideSelectPin);

const int nfcTimeout = 125;

bool hasInsideNFC = false;
bool hasOutsideNFC = false;
bool insideNFCDetected = false;
bool outsideNFCDetected = false;

LED insideNFCStateLED(insideNFCStateLEDPin, LOW);
LED outsideNFCStateLED(outsideNFCStateLEDPin, HIGH);
LED doorOpenStateLED(doorOpenStateLEDPin);

// speaker
const bool useHalfSteps = false;
const int stepsPerRevolution = useHalfSteps ? 96 : 48;
Stepper lockStepper(stepPin, dirPin, enablePin, stepsPerRevolution);

Button calibrationButton(calibrationButtonPin, true);
Button hallSensorButton(hallSensorPin, true);
// Button insideMotionSensor(insideMotionSensorPin);
// Button outsideMotionSensor(outsideMotionSensorPin);
// Button insideLockSwitch(insideLockSwitchPin, true);
// Button outsideLockSwitch(outsideLockSwitchPin, true);

// OLED display
const int I2C_SDA = 21;
const int I2C_SCL = 22;
TwoWire I2C1 = TwoWire(0); //I2C1 bus

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


    insideNFCStateLED.init();
    outsideNFCStateLED.init();
    doorOpenStateLED.init();

    calibrationButton.init();
    calibrationButton.addCallback([](Button::Value v) { if (v) calibrate(); } );

    // lockStepper.init();

    Serial.println("Done with first part of setup");

    if (Serial)
        Serial.println("Initializing inside NFC reader");
    insideNFC.begin();
    hasInsideNFC = insideNFC.checkNFCInterface();
    // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    if (!hasInsideNFC)
        if (Serial)
            Serial.println("Error initializing inside NFC reader");

    if (Serial)
        Serial.println("Initializing outside NFC reader");
    outsideNFC.begin();
    hasOutsideNFC = outsideNFC.checkNFCInterface();
    
    // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    // outsideNFC.setPassiveActivationRetries(0xFF);
    if (!hasOutsideNFC)
        if (Serial)
            Serial.println("Error initializing inside NFC reader");
}

void scanInputs()
{
    // TODO: check NFC IRQ states
    // insideNFCDetected = digitalRead(PN532_insideIRQ);
    // outsideNFCDetected = digitalRead(PN532_insideIRQ);

    // TODO: read hall-effect state
    calibrationButton.visit();
    // insideMotionSensor.visit();
    // outsideMotionSensor.visit();
    // insideLockSwitch.visit();
    // outsideLockSwitch.visit();
    hallSensorButton.visit();
}

void printOutputs()
{
    doorOpenStateLED.Set(hallSensorButton.getValue());
    if (Serial) {
        Serial.print("\tinside hall:");
        Serial.print(hallSensorButton.getValue());
        Serial.println("");
    }
}

void loop()
{
    // TODO: use sleep mode and wake on interrupt.


    // scanInputs();
    // printOutputs();

    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
    uint8_t uidLength; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // auto calibrate = calibrationButton.buttonPressed();
    auto calibrate = calibrationButton.getValue();
    if (calibrate) {
        // For now, just sets zero pos
        lockStepper.reset();
    } else {
    }

    // auto lockPos = getLockPos();
    // advanceLockToPos(lockPos);

    // if (calibrationState) {
    //     Serial.println("Scan a card");
    //     digitalWrite(lockStateLEDPin, 0);
    //     auto success = readTag(nfc, uid, &uidLength, nfcTimeout);
    //     if (success) {
    //         processTag(uid, uidLength);
    //         advanceLock();
    //     } else {
    //         Serial.println(
    //             "This doesn't seem to be an NTAG203 tag (UUID length != 7 bytes)!");
    //     }

    //     digitalWrite(lockStateLEDPin, 1);
    // }

    if (hasInsideNFC) {
        if (insideNFC.readTag(uid, &uidLength, nfcTimeout)) {
            if (Serial)
                Serial.println("INSIDE!");
            insideNFC.processTag(uid, uidLength);
            insideNFCStateLED.Set(HIGH);
        } else {
            insideNFCStateLED.Set(LOW);
        }
    }

    if (hasOutsideNFC) {
        if (outsideNFC.readTag(uid, &uidLength, nfcTimeout)) {
            if (Serial)
                Serial.println("OUTSIDE!");
            outsideNFC.processTag(uid, uidLength);
            outsideNFCStateLED.Set(HIGH);
        } else {
            outsideNFCStateLED.Set(LOW);
        }
    }
    // waitForKeypress();
    delay(20);
}

//
// 
//

struct Event {};

class CatDoorController
{
    // TODO: add state machine

    public:
    CatDoorController() {}

    void feed(const Event& event);
};

}
