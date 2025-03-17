#ifndef __POT__
#define __POT__

#include "utils/AsyncFSM.h"

#define POT_MOVING 2

/*
 * La classe Pot permette di creare un handler del componente Potenziometro. Questo fa si che quando viene registrato un cambiamento
 * della posizione del potenziometro, venga creato un evento. Questo nel caso di stato MANUAL va a modificare l'ampiezza dell'angolo
 * della valvola.
 */

class Pot : public EventSource
{
public:
    virtual bool isMoving() = 0;
};

class PotMoving : public Event
{
public:
    PotMoving(Pot *source) : Event(POT_MOVING)
    {
        this->source = source;
    }

    Pot *getSource()
    {
        return source;
    }

private:
    Pot *source;
};

#endif