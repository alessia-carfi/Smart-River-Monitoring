#ifndef __LED__
#define __LED__

class Led
{
private:
    int pin;
    bool turnedOff;

public:
    Led(int pin);
    void on();
    void off();
    void blink();
    bool status();
};

#endif
