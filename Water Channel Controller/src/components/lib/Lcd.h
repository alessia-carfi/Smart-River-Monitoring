#ifndef __LCDMONITOR__
#define __LCDMONITOR__


/*
* La classe LCdMonitor permette di visualizzare su un lcd 16x2 lo stato della macchina e l'angolo di apertura della valvola.
*/

class LcdMonitor {

public:
  virtual void on() = 0;
  virtual void write(const char str[], int angle) = 0;
  virtual ~LcdMonitor() {}
};

#endif
