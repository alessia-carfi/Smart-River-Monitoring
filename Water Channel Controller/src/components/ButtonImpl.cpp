#include "model/ButtonImpl.h"
#include "Arduino.h"

#define DEBOUNCING_TIME 50

ButtonImpl::ButtonImpl(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT);
    bindInterrupt(pin);
    lastEventTime = millis();
}

bool ButtonImpl::isPressed()
{
    return digitalRead(pin) == HIGH;
}

void ButtonImpl::notifyInterrupt(int pin)
{
    long curr = millis();
    if (curr - lastEventTime > DEBOUNCING_TIME)
    {
        lastEventTime = curr;
        Event *ev;
        if (isPressed())
        {
            ev = new ButtonPressed(this);
        }
        this->generateEvent(ev);
    }
}