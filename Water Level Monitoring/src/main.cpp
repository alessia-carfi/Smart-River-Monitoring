#include "model/wifisetup.h"

WifiSetup *wifisetup;

void setup()
{
  wifisetup = new WifiSetup();
  wifisetup->setupMQTT();
}

void loop()
{
  wifisetup->loopMQTT();
}
