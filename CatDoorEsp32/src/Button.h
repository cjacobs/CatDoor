//

// TODO: add callback for button-change events

class Button {
private:
    int pin;
    uint16_t history = 0;
    unsigned long T1 = 0;
    bool nc = false; // normally-closed
    bool buttonState = false;
    bool buttonWasPressed = false;
    int timeInterval = 0;

public:
    Button(int pin, bool normally_closed = false, int timeInterval = 5)
        : pin(pin)
        , nc(normally_closed)
        , timeInterval(timeInterval)
    {
    }

    void init()
    {
        pinMode(pin, INPUT_PULLUP);
    }

    bool readPin()
    {
        auto val = digitalRead(pin);
        if (nc)
            return !val;
        return val;
    }

    bool buttonPressed()
    {
        if (buttonWasPressed)
        {
            buttonWasPressed = false;
            return true;
        }
        return false;
    }

    void updateButtonState(bool newState)
    {
        if (!buttonState && newState)
        {
            buttonWasPressed = true;
        }
        
        buttonState = newState;
    }

    void scan()
    {        
        history <<= 1;
        history |= readPin();
        int sumStates = __builtin_popcount(history);
        if (sumStates >= 10)
            updateButtonState(true);
        else if (sumStates <= 4)
            updateButtonState(false);
    }

    bool read()
    {
        auto T2 = millis();
        if ((T2 - T1) >= timeInterval) {
            scan();
            T1 = T2;
        }

        if (buttonState) {
            history = 0;
            buttonState = false;
            return true;
        }
        return false;
    }
};
