#include <Arduino.h>

#include "model/Sonar.h"

Sonar::Sonar(int trig, int echo)
{
    this->trig = trig;
    this->echo = echo;

    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
}

bool Sonar::isIn()
{
    return getDistance() <= MINDIST && getDistance() > 0.1;
}

bool Sonar::isOut()
{
    return getDistance() >= MAXDIST;
}

float Sonar::getDistance()
{
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    long duration = pulseIn(echo, HIGH);
    Serial.println(duration);
    int distance = duration * 0.034 / 2;
    
    return distance;
}
