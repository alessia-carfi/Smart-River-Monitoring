#ifndef __TEMP__
#define __TEMP__

const float MAXTEMP = 90.0;

class Temp {
    private:
        int pin;

    public:
        Temp(int pin);
        float getTemp();
        bool isOverTemp();
};

#endif
