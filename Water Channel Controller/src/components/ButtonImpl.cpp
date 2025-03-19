#include "model/ButtonImpl.h"
#include "Arduino.h"

#define DEBOUNCING_TIME 20

ButtonImpl::ButtonImpl(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP);
    bindInterrupt(pin);
    lastEventTime = millis();
}

bool ButtonImpl::isPressed()
{
    return digitalRead(pin) == LOW;
}

void ButtonImpl::notifyInterrupt(int pin)
{
    unsigned long curr = millis();
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