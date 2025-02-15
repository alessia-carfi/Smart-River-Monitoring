#include <Arduino.h>

#include "model/Temp.h"


Temp::Temp(int pin){
    this->pin = pin;
}

float Temp::getTemp(){
    return analogRead(pin) * 0.48828125;
}

bool Temp::isOverTemp(){
    return getTemp() > MAXTEMP;
}