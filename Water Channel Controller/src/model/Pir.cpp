#include <Arduino.h>

#include "model/Pir.h"


Pir::Pir(int pin){
    this->pin = pin;
    pinMode(pin, INPUT);
    delay(5000);
}

bool Pir::isDetected(){
    return digitalRead(pin);
}