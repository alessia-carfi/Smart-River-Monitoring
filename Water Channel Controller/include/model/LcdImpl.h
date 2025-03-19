#ifndef __LCDMONITORIMPL__
#define __LCDMONITORIMPL__

#include "Lcd.h"
#include <LiquidCrystal.h>
//#include <LiquidCrystal_I2C.h>

class LcdMonitorImpl : public LcdMonitor
{
public:
  LcdMonitorImpl() : lcd(rs, en, d4, d5, d6, d7) {}
  //LcdMonitorImpl() : lcd(0x27, 20, 4) {}

  void on() override;
  void write(const char str[], int angle) override;

private:
  int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
  LiquidCrystal lcd;
  //LiquidCrystal_I2C lcd;
};

#endif
