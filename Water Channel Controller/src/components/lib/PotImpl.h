#ifndef __POTIMPL__
#define __POTIMPL__

#include "Pot.h"

class PotImpl : public Pot {

    public:
        PotImpl(int pin);
        virtual bool isMoving();
        virtual void notifyInterrupt(int pin);
        long getValue();
    private:
        int pin;
        long lastValue;
};

#endif