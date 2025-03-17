#ifndef __SERVOMOTOR__
#define __SERVOMOTOR__

/*
* La classe ServoMotor permette di modificare l'angolo di apertura della valvola, rappresentata dal componente Servo Motore.
*/

class ServoMotor {

public:
  virtual void on() = 0;
  virtual void setPosition(int angle) = 0;
  virtual void off() = 0;
};

#endif