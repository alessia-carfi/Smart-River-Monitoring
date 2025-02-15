#ifndef __LCD__
#define __LCD__

#include <WString.h>
#include <LiquidCrystal_I2C.h>


class Lcd {
private:
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

public:
    Lcd();
    void on();
    void off();
    void say(String msg);
};

#endif
