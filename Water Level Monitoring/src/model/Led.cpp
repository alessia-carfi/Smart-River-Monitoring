#include <Arduino.h>

#include "model/Led.h"


Led::Led(int pin) {
    turnedOff = false;
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void Led::on(){
    digitalWrite(pin, HIGH);
}

void Led::off(){
    digitalWrite(pin, LOW);
}

void Led::blink(){
    if (turnedOff) {
        on();
        turnedOff = false;
    } else {
        off();
        turnedOff = true;
    }
}