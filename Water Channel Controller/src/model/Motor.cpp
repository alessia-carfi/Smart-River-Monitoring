#include <Arduino.h>

#include "model/Motor.h"

Motor::Motor(int pin){
    this->pin = pin;
}

void Motor::on(){
   servo.attach(pin);
}

void Motor::off(){
    servo.detach();
}

void Motor::setPosition(unsigned long t){
    servo.write(t);
}