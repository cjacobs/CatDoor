#pragma once
class Stepper
{
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
    int currPos; // in steps
    Stepper::Dir dir;

  public:
    Stepper(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin, int stepsPerRevolution)
        : stepPin(stepPin), dirPin(dirPin), enablePin(enablePin), stepsPerRevolution(stepsPerRevolution)
    {
    }

    void init()
    {
        pinMode(stepPin, OUTPUT);
        pinMode(dirPin, OUTPUT);
        pinMode(enablePin, OUTPUT);
        digitalWrite(enablePin, LOW);

        digitalWrite(dirPin, LOW);
        digitalWrite(stepPin, LOW);
    }

    void enable()
    {
        digitalWrite(enablePin, LOW);
    }

    void disable()
    {
        digitalWrite(enablePin, HIGH);
    }

    void reset()
    {
        currPos = 0;
    }

    void step()
    {
        int numSteps = 1;
        pulsePin(stepPin);
        currPos += (int)dir * numSteps;

        currPos = (currPos + stepsPerRevolution) % stepsPerRevolution;
    }

    void setDir(Dir d)
    {
        setPin(dirPin, (int)d > 0);
        dir = d;
    }

    Dir getDir()
    {
        return dir;
    };

    int getPos()
    {
        return currPos;
    }

    // 0 <= pos <= numStepsPerRevolution-1
    void goToPos(int pos, int delayTime = 1)
    {
        pos = pos % stepsPerRevolution;

        Serial.print("currPos: ");
        Serial.print(currPos);
        Serial.print(" newPos: ");
        Serial.print(pos);

        if (pos == currPos)
        {
            Serial.println(" <nothing>");
            return;
        }

        int diff = ((pos + stepsPerRevolution) - currPos) % stepsPerRevolution;
        int steps = 0;
        Serial.print(" num steps: ");
        Serial.print(diff);
        Serial.println("");
        while (steps < diff)
        {
            step();
            steps += 1;
            delay(delayTime);
        }

        currPos = (currPos + steps) % stepsPerRevolution;
        Serial.print(" final pos: ");
        Serial.print(currPos);
        Serial.println("");
    }

  private:
    void pulsePin(uint8_t pin, bool highPulse = true, int delayTime = 2)
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
