//
// 
//

class StateMachine {
    typedef void (StateMachine::*StateFunction)(void);

public:
    StateMachine()
    {
        SetState(&StateMachine::Home);
    }

    void SetState(StateMachine::StateFunction stateFn)
    {
        currStateFn = stateFn;
    }

    void ProcessEvent()
    {

    }

    void Pulse() // need better name...
    {
        (this->*currStateFn)();
    }

    void Home()
    {
        bool resetButtonHit = true;
        bool calibrateButtonHit = true;

        // wait for various events
        if (resetButtonHit) {
            SetState(&StateMachine::Reset);
        } else if (calibrateButtonHit) {
            SetState(&StateMachine::Calibrate);
        }
    }

    void Calibrate()
    {
        // move motor, waiting for (say) both limit switches to be 'on'
        // then reset stepper pos to origin

        SetState(&StateMachine::Home);
    }

    void Reset()
    {
        // do something or other
        SetState(&StateMachine::Home);
    }

    void MotionDetected()
    {
        // nothing really
        SetState(&StateMachine::WaitForTag);
    }

    void WaitForTag()
    {
        // wait awhile trying to sense a tag

        bool gotCorrectCat = true;
        if (gotCorrectCat) {
            SetState(&StateMachine::Unlock);
        } else {
            SetState(&StateMachine::Home);
        }
    }

    void Unlock()
    {

        // move lock mechanism if necessary

        // wait for a while

        SetState(&StateMachine::ResetLock);
    }

    void ResetLock()
    {
        SetState(&StateMachine::Home);
    }

private:
    StateFunction currStateFn = nullptr;
};
