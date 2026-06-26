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
#include <Config.h>

struct Event {};

class CatDoorController
{
    // TODO: add state machine

public:
    CatDoorController(Config cfg = Config{})
        : config(cfg)
        , insideNFC(config.insideSelectPin)
        , outsideNFC(config.outsideSelectPin)
        , insideNFCStateLED(config.insideNFCStateLEDPin, LOW)
        , outsideNFCStateLED(config.outsideNFCStateLEDPin, HIGH)
        , doorOpenStateLED(config.doorOpenStateLEDPin)
        , lockStepper(config.stepPin, config.dirPin, config.enablePin, config.stepsPerRevolution)
        , calibrationButton(config.calibrationButtonPin, true)
        , hallSensorButton(config.hallSensorPin, true)
        , I2C1(0)
    {}

    void setup()
    {
        Serial.begin(115200);
        while (!Serial)
            delay(10); // for Leonardo/Micro/Zero

        if (Serial)
            Serial.println("Hello!");

        insideNFCStateLED.init();
        outsideNFCStateLED.init();
        doorOpenStateLED.init();

        calibrationButton.init();
        calibrationButton.addCallback([this](Button::Value v) { if (v) calibrate(); });

        // lockStepper.init();

        Serial.println("Done with first part of setup");

        if (Serial)
            Serial.println("Initializing inside NFC reader");
        insideNFC.begin();
        hasInsideNFC = insideNFC.checkNFCInterface();
        if (!hasInsideNFC)
            if (Serial)
                Serial.println("Error initializing inside NFC reader");

        if (Serial)
            Serial.println("Initializing outside NFC reader");
        outsideNFC.begin();
        hasOutsideNFC = outsideNFC.checkNFCInterface();
        // outsideNFC.setPassiveActivationRetries(0xFF);
        if (!hasOutsideNFC)
            if (Serial)
                Serial.println("Error initializing outside NFC reader");
    }

    void loop()
    {
        // TODO: use sleep mode and wake on interrupt.

        // scanInputs();
        // printOutputs();

        uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
        uint8_t uidLength;

        auto doCalibrate = calibrationButton.getValue();
        if (doCalibrate) {
            lockStepper.reset();
        }

        if (hasInsideNFC) {
            if (insideNFC.readTag(uid, &uidLength, config.nfcTimeout)) {
                if (Serial)
                    Serial.println("INSIDE!");
                insideNFC.processTag(uid, uidLength);
                insideNFCStateLED.Set(HIGH);
            } else {
                insideNFCStateLED.Set(LOW);
            }
        }

        if (hasOutsideNFC) {
            if (outsideNFC.readTag(uid, &uidLength, config.nfcTimeout)) {
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

    void feed(const Event& event);

private:
    Config config;

    NFCReader insideNFC;
    NFCReader outsideNFC;

    bool hasInsideNFC = false;
    bool hasOutsideNFC = false;
    bool insideNFCDetected = false;
    bool outsideNFCDetected = false;

    LED insideNFCStateLED;
    LED outsideNFCStateLED;
    LED doorOpenStateLED;

    Stepper lockStepper;

    Button calibrationButton;
    Button hallSensorButton;

    // Button insideMotionSensor;
    // Button outsideMotionSensor;
    // Button insideLockSwitch;
    // Button outsideLockSwitch;

    TwoWire I2C1;

    void calibrate()
    {
        // ...
    }

    void scanInputs()
    {
        // TODO: check NFC IRQ states
        calibrationButton.visit();
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

        Serial.println("\n\nSend a character to scan another tag!!!!");
        Serial.flush();
        while (!Serial.available())
            ;
        while (Serial.available()) {
            Serial.read();
        }
        Serial.flush();
    }
};
