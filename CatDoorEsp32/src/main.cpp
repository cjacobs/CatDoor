// ESP32C version

#include <SPI.h>
#include <Wire.h>

#include <Adafruit_PN532.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Button.h>
#include <NFCHelper.h>
// #include <StateMachine.h>
#include <Stepper.h>
// Arduino reference:
// https://docs.arduino.cc/language-reference/

const int INSIDE = 1;
const int OUTSIDE = 0;

const int insideSelect = 5;
const int outsideSelect = 17;
const int PN532_MOSI = 23;
const int PN532_MISO = 19;
const int PN532_SCK = 18;

// Use this line for a breakout with a software SPI connection (recommended):
Adafruit_PN532 insideNFC(insideSelect);
Adafruit_PN532 outsideNFC(outsideSelect);

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
const int insideNFCStateLedPin = 32;
const int outsideNFCStateLedPin = 33;
const int doorOpenStateLedPin = 27;

const bool useHalfSteps = false;
const int stepsPerRevolution = useHalfSteps ? 96 : 48;

int totalSteps = 0;
uint8_t dir = 0;
Stepper lockStepper(stepPin, dirPin, enablePin, stepsPerRevolution);

Button calibrationButton(calibrationButtonPin, true);
Button insideMotionSensor(insideMotionSensorPin);
Button outsideMotionSensor(outsideMotionSensorPin);
Button insideLockSwitch(insideLockSwitchPin, true);
Button outsideLockSwitch(outsideLockSwitchPin, true);

int leftHallEffectVal = 0;
int rightHallEffectVal = 0;

//
//
//
// display

const int I2C_SDA = 21;
const int I2C_SCL = 22;
TwoWire I2C1 = TwoWire(0); //I2C1 bus




#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C1, OLED_RESET);


void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}
//
//
//



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
    int insideLock = insideLockSwitch.readPin();
    int outsideLock = outsideLockSwitch.readPin();
    int pos = (insideLock ? quarterTurn : 0) + (outsideLock ? halfTurn : 0);
    return pos;
}

void advanceLockToPos(int pos)
{
    int currPos = totalSteps;
    if (pos == currPos)
        return;

    int diff = ((pos + stepsPerRevolution) - currPos) % stepsPerRevolution;

    if (Serial) {
        Serial.print("currPos: ");
        Serial.println(currPos);
        Serial.print("moving to: ");
        Serial.println(pos);
        Serial.print("diff: ");
        Serial.println(diff);
    }

    int steps = 0;
    while (steps < diff) {
        // lockStepper.step();
        steps += 1;
        delay(4);
    }

    totalSteps = (totalSteps + steps) % stepsPerRevolution;
    if (Serial) {
        Serial.print("total steps: ");
        Serial.println(totalSteps);
    }
}

void SetLED(int pin, int val)
{
    // maybe negate
    digitalWrite(pin, val);
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


            // Wait for display
    delay(500);

    I2C1.begin(I2C_SDA, I2C_SCL); // Start I2C1 on pins 21 and 22

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
            Serial.println(F("SSD1306 allocation failed"));
            for(;;); // Don't proceed, loop forever
        }
        
        Serial.println("Initialized!");


    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(2000); // Pause for 2 seconds

      // Clear the buffer
    display.clearDisplay();

    // Draw a single pixel in white
    display.drawPixel(10, 10, SSD1306_WHITE);
    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    display.display();
    delay(2000);


    testscrolltext();
    delay(2000);
    return;





    // pinMode(leftHallSensorPin, INPUT);
    // pinMode(rightHallSensorPin, INPUT);
    // pinMode(doorOpenStateLedPin, OUTPUT);
    // SetLED(doorOpenStateLedPin, HIGH);
    
    // pinMode(insideNFCStateLedPin, OUTPUT);
    // digitalWrite(insideNFCStateLedPin, HIGH);
    // pinMode(outsideNFCStateLedPin, OUTPUT);
    // digitalWrite(outsideNFCStateLedPin, HIGH);
    // calibrationButton.init();
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
    hasInsideNFC = checkNFCInterface(insideNFC);
    // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    if (!hasInsideNFC)
        if (Serial)
            Serial.println("Error initializing inside NFC reader");

    if (Serial)
        Serial.println("Initializing outside NFC reader");
    outsideNFC.begin();
    hasOutsideNFC = checkNFCInterface(outsideNFC);
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
    calibrationButton.scan();
    insideMotionSensor.scan();
    outsideMotionSensor.scan();
    insideLockSwitch.scan();
    outsideLockSwitch.scan();

    // leftHallEffectVal = analogRead(leftHallSensorPin);
    // rightHallEffectVal = analogRead(rightHallSensorPin);
    leftHallEffectVal = digitalRead(leftHallSensorPin);
    rightHallEffectVal = digitalRead(rightHallSensorPin);
}

void printOutputs()
{
    SetLED(doorOpenStateLedPin, !leftHallEffectVal);


    // move this stuff to scanInputs?
    // int calibration = calibrationButton.readPin();
    // int insideLock = insideLockSwitch.readPin();
    // int outsideLock = outsideLockSwitch.readPin();
    // int insideMotion = insideMotionSensor.readPin();
    // int outsideMotion = outsideMotionSensor.readPin();

    // printIfElse(calibration, "cal: ", "X", " ");
    // printIfElse(insideLock, "\tinside lock: ", "  locked", "unlocked");
    // printIfElse(outsideLock, "\toutside lock: ", "  locked", "unlocked");
    // printIfElse(insideMotion, "\t", "inside motion", "             ");
    // printIfElse(outsideMotion, "\t", "outside motion", "              ");
    if (Serial) {
        Serial.print("\tinside hall:");
        Serial.print(!leftHallEffectVal);
        // Serial.print("\toutside hall:");
        // Serial.print(rightHallEffectVal);

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
    auto calibrate = calibrationButton.readPin();
    if (calibrate) {
        // For now, just sets zero pos
        totalSteps = 0;
    } else {
    }

    // auto lockPos = getLockPos();
    // advanceLockToPos(lockPos);

    // if (calibrationState) {
    //     Serial.println("Scan a card");
    //     digitalWrite(lockStateLedPin, 0);
    //     auto success = readTag(nfc, uid, &uidLength, nfcTimeout);
    //     if (success) {
    //         processTag(uid, uidLength);
    //         advanceLock();
    //     } else {
    //         Serial.println(
    //             "This doesn't seem to be an NTAG203 tag (UUID length != 7 bytes)!");
    //     }

    //     digitalWrite(lockStateLedPin, 1);
    // }

    if (hasInsideNFC) {
        if (readTag(insideNFC, uid, &uidLength, nfcTimeout)) {
            if (Serial)
                Serial.println("INSIDE!");
            // processTag(uid, uidLength);
            // SetLED(insideNFCStateLedPin, LOW);
        } else {
            // SetLED(insideNFCStateLedPin, HIGH);
        }
    }

    if (hasOutsideNFC) {
        if (readTag(outsideNFC, uid, &uidLength, nfcTimeout)) {
            if (Serial)
                Serial.println("OUTSIDE!");
            // processTag(uid, uidLength);
            // SetLED(outsideNFCStateLedPin, LOW);
        } else {
            // SetLED(outsideNFCStateLedPin, HIGH);
        }
    }
    // waitForKeypress();
    delay(20);
}
