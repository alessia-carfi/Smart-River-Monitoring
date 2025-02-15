#ifndef _WIFISETUP_
#define _WIFISETUP_

#define MSG_BUFFER_SIZE 50

const char *ssid = "ssid";
const char *password = "password";

/* MQTT server address for now I keep this */
const char *mqtt_server = "broker.mqtt-dashboard.com";

/* MQTT topic idk what a topic is */
const char *topic = "esiot-2023";

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
