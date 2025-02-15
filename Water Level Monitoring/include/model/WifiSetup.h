#include <Arduino.h>

#ifndef _WIFISETUP_
#define _WIFISETUP_

#define MSG_BUFFER_SIZE 50

class WifiSetup
{
public:
    void setup_wifi();
    void reconnect();
    void setupMQTT();
    void loopMQTT();
    static void callback(char *topic, byte *payload, unsigned int length);
};

#endif
