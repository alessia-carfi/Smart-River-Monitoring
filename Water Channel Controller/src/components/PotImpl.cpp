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
    if (abs(curr - lastValue) >= 3)
    {
        Serial.println(lastValue);  
        lastValue = round(curr); 
        Serial.println(lastValue);
        delay(2000);
        return true;
    } else {
        return false;
    }
}

int Pot::getValue()
{
    return map(lastValue, 0, 1023, 0, 180);
}