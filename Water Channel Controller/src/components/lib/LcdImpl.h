#ifndef __LCDMONITORIMPL__
#define __LCDMONITORIMPL__

#include "lcd.h"
#include <LiquidCrystal.h>

class LcdMonitorImpl : public LcdMonitor {
  public:
    LcdMonitorImpl() : lcd(rs, en, d4, d5, d6, d7) {}

    void on() override;
    void write(const char str[], int angle) override;

  private:
    int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
    LiquidCrystal lcd;
};

#endif
