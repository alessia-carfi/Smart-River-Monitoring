#include "lib/PotImpl.h"
#include "Arduino.h"

PotImpl::PotImpl(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
    bindInterrupt(pin);
    lastValue = analogRead(pin);
}

bool PotImpl::isMoving() {
    long curr = analogRead(pin);
    if (abs(curr - lastValue) > 10) {
        Serial.println(curr);
        lastValue = curr; 
        return true;
    }
    return false;
}

void PotImpl::notifyInterrupt(int pin) {
    long curr = analogRead(pin);
    Event* ev;
    if(isMoving()) {
        lastValue = curr;
        ev = new PotMoving(this);
    }
    this->generateEvent(ev);
}

long PotImpl::getValue() {
    return map(analogRead(pin), 0, 1023, 0, 180);
}