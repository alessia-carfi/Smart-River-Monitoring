#ifndef __POT__
#define __POT__

/*
 * La classe Pot permette di creare un handler del componente Potenziometro. Questo fa si che quando viene registrato un cambiamento
 * della posizione del potenziometro, venga creato un evento. Questo nel caso di stato MANUAL va a modificare l'ampiezza dell'angolo
 * della valvola.
 */
class Pot {
    public:
        Pot(int pin);
        virtual bool isMoving();
        int getValue();
    private:
        int pin;
        int lastValue;
};

#endif