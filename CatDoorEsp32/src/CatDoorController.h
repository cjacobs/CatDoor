//
// "business logic" for the cat door
//

// TODO: abstract out all the Arduino-specific stuff

#include <SPI.h>
#include <Wire.h>
#include <esp_sleep.h>

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
        calibrationButton.addCallback([this](Button::Value v) { if (v) calibrate(); });

        // lockStepper.init();

        if (config.useSleep)
            configureSleepWakeup();

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
                Serial.println("Error initializing outside NFC reader");
    }

    void loop()
    {
        scanInputs();
        printOutputs();

        uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
        uint8_t uidLength; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

        // auto doCalibrate = calibrationButton.buttonPressed();
        auto doCalibrate = calibrationButton.getValue();
        if (doCalibrate) {
            // For now, just sets zero pos
            lockStepper.reset();
        } else {
        }

        // auto lockPos = getLockPos();
        // advanceLockToPos(lockPos);

        // if (calibrationState) {
        //     Serial.println("Scan a card");
        //     digitalWrite(lockStateLEDPin, 0);
        //     auto success = readTag(nfc, uid, &uidLength, config.nfcTimeout);
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

        if (config.useSleep)
            esp_light_sleep_start();
        else
            delay(config.eventLoopDelay);

        static int count = 0;
        Serial.println(count++);
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

    // OLED display
    // TODO: put somewhere else
    TwoWire I2C1;

    void calibrate()
    {
        // ...
    }

    void configureSleepWakeup()
    {
        // ESP-IDF 4.x has no "wake when ANY pin goes LOW" mode for ext1 wakeup
        // (only ALL_LOW or ANY_HIGH). Since buttons are active-low and checked
        // every loop iteration, timer-only wakeup is sufficient.
        esp_sleep_enable_timer_wakeup(config.eventLoopDelay * 1000);
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
            Serial.print("\tinside hall: ");
            Serial.print(hallSensorButton.getValue());
            Serial.println("");
            
            Serial.print("\tcalibration: ");
            Serial.print(calibrationButton.getValue());
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
};
