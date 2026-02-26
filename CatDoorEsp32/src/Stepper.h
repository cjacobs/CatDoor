
class Stepper {
public:
    enum Dir
    {
        CW = 1,
        CCW = -1
    };

    private:
    uint8_t stepPin;
    uint8_t dirPin;
    uint8_t enablePin;

    int stepsPerRevolution;
    int currPos;
    Stepper::Dir dir;

public:

    Stepper(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin, int stepsPerRevolution)
        : stepPin(stepPin)
        , dirPin(dirPin)
        , enablePin(enablePin)
    {
    }

    void init()
    {
        Serial.println("S1");
        pinMode(stepPin, OUTPUT);
        Serial.println("S2");
        pinMode(dirPin, OUTPUT);
        Serial.println("S3");
        pinMode(enablePin, OUTPUT);
        digitalWrite(enablePin, 0);
        Serial.println("S4");
    }

    void step()
    {
        int numSteps = 1;
        pulsePin(stepPin);
        currPos += (int)dir * numSteps;

        currPos = (currPos+stepsPerRevolution) % stepsPerRevolution;
    }

    void setDir(Dir d)
    {
        setPin(dirPin, (int)d > 0);
        dir = d;
    }

    Dir getDir() { return dir; };

    int getPos() { return currPos; }

    void zeroCounter() { currPos = 0; }

    // 0 <= pos <= numStepsPerRevolution-1
    void goToPos(int pos, int delayTime=1)
    {
        int diff = pos - currPos;
    }

private:
    void pulsePin(uint8_t pin, bool highPulse = true, int delayTime = 4)
    {
        const uint8_t lo = highPulse ? 0 : 1;
        const uint8_t hi = !lo;
 
        digitalWrite(pin, lo);
        delay(delayTime);
        digitalWrite(pin, hi);
        delay(delayTime);
        digitalWrite(pin, lo);
    }

    void setPin(uint8_t pin, uint8_t value)
    {
        digitalWrite(pin, value);
    }
};
