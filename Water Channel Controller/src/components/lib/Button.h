#ifndef __BUTTON__
#define __BUTTON__

#include "fsm/asyncFSM.h"

#define BUTTON_PRESSED_EVENT 1

/*
* La classe Button permette di creare un handler del componente Bottone. Questo fa si che quando viene registrato un cambiamento
* della frequenza del pin del bottone, venga creato un evento. Il bottone premuto deve infatti permettere alla FSM di cambiare lo
* stato della macchina da automatico a manuale.
*/

class Button :  public EventSource {
    public:
        virtual bool isPressed() = 0;
};

class ButtonPressed: public Event {
    public:
        ButtonPressed(Button* source) : Event(BUTTON_PRESSED_EVENT) {
            this->source = source;
        }

        Button* getSource() {
            return source;
        }
    private:
        Button* source;
};

#endif