#ifndef __GATE__
#define __GATE__

#include "utils/ServoTimer2.h"

class Motor {
    private:
        int pin;
        ServoTimer2 servo; 

    public:
        Motor(int pin);
        void on();
        void off();
        void setPosition(unsigned long t);
};

#endif
