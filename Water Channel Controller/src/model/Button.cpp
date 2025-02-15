#include <Arduino.h>
#include "model/Button.h"

Button::Button(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
}

bool Button::isPressed(){
    if(digitalRead(pin) == HIGH){
        return true;
    }else{
        return false;
    }
}