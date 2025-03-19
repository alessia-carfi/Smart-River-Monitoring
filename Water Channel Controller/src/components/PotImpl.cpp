#include "model/Pot.h"
#include "Arduino.h"

Pot::Pot(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT);
    lastValue = analogRead(pin);
}

bool Pot::isMoving()
{
    int curr = analogRead(pin);
    if (abs(curr - lastValue) >= 5)
    {   
        lastValue = round(curr); 
        return true;
    } else {
        return false;
    }
}

int Pot::getValue()
{
    return map(lastValue, 0, 1023, 0, 180);
}