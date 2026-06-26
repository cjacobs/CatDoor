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

struct Event {};

class CatDoorController
{
    // TODO: add state machine

public:
    CatDoorController()
        : insideNFC(insideSelectPin)
        , outsideNFC(outsideSelectPin)
        , insideNFCStateLED(insideNFCStateLEDPin, LOW)
        , outsideNFCStateLED(outsideNFCStateLEDPin, HIGH)
        , doorOpenStateLED(doorOpenStateLEDPin)
        , lockStepper(stepPin, dirPin, enablePin, stepsPerRevolution)
        , calibrationButton(calibrationButtonPin, true)
        , hallSensorButton(hallSensorPin, true)
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

        // auto doCalibrate = calibrationButton.buttonPressed();
        auto doCalibrate = calibrationButton.getValue();
        if (doCalibrate) {
            lockStepper.reset();
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

    void feed(const Event& event);

private:
    // NFC SPI pins
    static constexpr int outsideSelectPin = 5;
    static constexpr int insideSelectPin = 17;
    static constexpr int PN532_MOSI = 23;
    static constexpr int PN532_MISO = 19;
    static constexpr int PN532_SCK = 18;

    // sensors
    static constexpr int calibrationButtonPin = 36;
    static constexpr int hallSensorPin = 16;

    // motor
    static constexpr int stepPin = 4;
    static constexpr int dirPin = 0;
    static constexpr int enablePin = 16;

    // display / output
    static constexpr int insideNFCStateLEDPin = 32;
    static constexpr int outsideNFCStateLEDPin = 33;
    static constexpr int doorOpenStateLEDPin = 27;

    static constexpr int speakerPin = 0;

    // I2C display
    static constexpr int I2C_SDA = 21;
    static constexpr int I2C_SCL = 22;

    static constexpr int nfcTimeout = 125;
    static constexpr bool useHalfSteps = false;
    static constexpr int stepsPerRevolution = useHalfSteps ? 96 : 48;

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
