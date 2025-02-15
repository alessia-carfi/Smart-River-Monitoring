#ifndef __PIR__
#define __PIR__

class Pir {
    private:
        int pin;
    public:
        Pir(int pin);
        bool isDetected(); 
};

#endif
