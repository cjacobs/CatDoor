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
#include <NFCHelper.h>
#include <Stepper.h>

namespace Controller
{
const int insideSelectPin = 5;
const int outsideSelectPin = 17;

// TODO: pass in pins?
const int PN532_MOSI = 23;
const int PN532_MISO = 19;
const int PN532_SCK = 18;

// Use this line for a breakout with a software SPI connection (recommended):
NFCReader insideNFC(insideSelectPin);
NFCReader outsideNFC(outsideSelectPin);

const int nfcTimeout = 125;

bool hasInsideNFC = false;
bool hasOutsideNFC = false;
bool insideNFCDetected = false;
bool outsideNFCDetected = false;

// CatDoor pins

// sensors
const int calibrationButtonPin = 36;
const int insideLockSwitchPin = 39;
const int outsideLockSwitchPin = 34;
const int insideMotionSensorPin = 35;
const int outsideMotionSensorPin = 14;
const int leftHallSensorPin = 16;
const int rightHallSensorPin = 17;

// motor
const int stepPin = 4;
const int dirPin = 0;
const int enablePin = 16;

// display
const int insideNFCStateLEDPin = 32;
const int outsideNFCStateLEDPin = 33;
const int doorOpenStateLEDPin = 27;


LED insideNFCStateLED(insideNFCStateLEDPin);
LED outsideNFCStateLED(outsideNFCStateLEDPin);
LED doorOpenStateLED(doorOpenStateLEDPin);

const bool useHalfSteps = false;
const int stepsPerRevolution = useHalfSteps ? 96 : 48;
Stepper lockStepper(stepPin, dirPin, enablePin, stepsPerRevolution);

Button calibrationButton(calibrationButtonPin, true);
Button insideMotionSensor(insideMotionSensorPin);
Button outsideMotionSensor(outsideMotionSensorPin);
Button insideLockSwitch(insideLockSwitchPin, true);
Button outsideLockSwitch(outsideLockSwitchPin, true);
Button leftHallSensorButton(leftHallSensorPin, true);
Button rightHallSensorButton(rightHallSensorPin, true);

//

//
//
//
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

int getLockPos()
{
    int quarterTurn = stepsPerRevolution / 4;
    int halfTurn = stepsPerRevolution / 2;
    // open, open = 0
    // closed, open = 1
    // open, closed = 2
    // closed, closed = 3
    int insideLock = insideLockSwitch.getValue();
    int outsideLock = outsideLockSwitch.getValue();
    int pos = (insideLock ? quarterTurn : 0) + (outsideLock ? halfTurn : 0);
    return pos;
}

void advanceLockToPos(int pos)
{
    lockStepper.goToPos(pos);
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


    calibrationButton.init();
    calibrationButton.addCallback([](Button::Value v) { if (v) calibrate(); } );
    // insideMotionSensor.init();
    // outsideMotionSensor.init();
    // insideLockSwitch.init();
    // outsideLockSwitch.init();

    // lockStepper.init();

    Serial.println("Done with first part of setup");


    // TODO: refactor this
    // pinMode(insideSelect, OUTPUT);
    // digitalWrite(insideSelect, HIGH);

    // pinMode(outsideSelect, OUTPUT);
    // digitalWrite(outsideSelect, HIGH);

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
    insideMotionSensor.visit();
    outsideMotionSensor.visit();
    insideLockSwitch.visit();
    outsideLockSwitch.visit();
    leftHallSensorButton.visit();
    rightHallSensorButton.visit();
}

void printOutputs()
{
    doorOpenStateLED.Set(leftHallSensorButton.getValue());

    // move this stuff to scanInputs?
    // int calibration = calibrationButton.getValue();
    // int insideLock = insideLockSwitch.getValue();
    // int outsideLock = outsideLockSwitch.getValue();
    // int insideMotion = insideMotionSensor.getValue();
    // int outsideMotion = outsideMotionSensor.getValue();

    // printIfElse(calibration, "cal: ", "X", " ");
    // printIfElse(insideLock, "\tinside lock: ", "  locked", "unlocked");
    // printIfElse(outsideLock, "\toutside lock: ", "  locked", "unlocked");
    // printIfElse(insideMotion, "\t", "inside motion", "             ");
    // printIfElse(outsideMotion, "\t", "outside motion", "              ");
    if (Serial) {
        Serial.print("\tinside hall:");
        Serial.print(leftHallSensorButton.getValue());
        // Serial.print("\toutside hall:");
        // Serial.print(rightHallSensorButton.getValue());

        Serial.println("");
    }
}

void loop()
{
    // scanInputs();
    // printOutputs();

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
            // processTag(uid, uidLength);
            // SetLED(insideNFCStateLEDPin, LOW);
        } else {
            // SetLED(insideNFCStateLEDPin, HIGH);
        }
    }

    if (hasOutsideNFC) {
        if (outsideNFC.readTag(uid, &uidLength, nfcTimeout)) {
            if (Serial)
                Serial.println("OUTSIDE!");
            // processTag(uid, uidLength);
            // SetLED(outsideNFCStateLEDPin, LOW);
        } else {
            // SetLED(outsideNFCStateLEDPin, HIGH);
        }
    }
    // waitForKeypress();
    delay(20);
}

//
// TODO: eventually migrate everything here
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
