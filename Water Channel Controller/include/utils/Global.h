#ifndef GLOBAL_H
#define GLOBAL_H

#define PINLED1 13
#define PINLED2 12
#define PINLED3 10
#define PINMOTOR 5
#define PINBUTTON 8
#define PINTEMP A3
#define PINPIR 9
#define ECHO 2
#define TRIG 4

#include <TimerOne.h>


enum State
{
    SleepState,
    DetectionState,
    ProcedingState,
    EntranceState,
    WashingState,
    EndState
};

extern State state;

extern int totalWashing;

extern bool isHere;
extern bool in;
extern bool out;

#endif