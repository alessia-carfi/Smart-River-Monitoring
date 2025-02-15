#include <Arduino.h>

#include "model/Lcd.h"


Lcd::Lcd() {
    lcd.init();
    lcd.backlight();
}

void Lcd::off() {
    lcd.noBacklight();
}

void Lcd::on() {
    lcd.backlight();
}

void Lcd::say(String msg) {
    lcd.clear();
    if(msg.length() > 16) {
        String part1 = msg.substring(0, 16);
        String part2 = msg.substring(16, msg.length());
        lcd.setCursor(0, 0);
        lcd.print(part1);
        lcd.setCursor(0, 1);
        lcd.print(part2);
    }
    lcd.setCursor(0, 0);
    lcd.print(msg);
}