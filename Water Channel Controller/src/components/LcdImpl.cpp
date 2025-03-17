#include "lib/LcdImpl.h"
#include "Arduino.h"

void LcdMonitorImpl::on() { 
    lcd.begin(16, 2);
}

void LcdMonitorImpl::write(const char str[], int angle) {
    int percentage = map(angle, 0, 180, 0, 100);
    char string[12];
    itoa(percentage, string, 10);

    lcd.clear();
    lcd.setCursor(0, 1);

    char output[30];
    snprintf(output, sizeof(output), "%s-%s%%", str, string);

    lcd.print(output);
}
