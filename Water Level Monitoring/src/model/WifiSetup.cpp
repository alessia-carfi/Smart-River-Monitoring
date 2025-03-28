#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "model/WifiSetup.h"
#include "model/Led.h"
#include "model/Sonar.h"
#include "utils/Global.h"

const char *ssid = "Galaxy S25";
const char *password = "Sc14092001";

/* MQTT server address for now I keep this */
const char *mqtt_server = "192.168.72.15";

const char *topic = "water-level";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsgTime = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;
unsigned long frequency = 10000;
Led *green = new Led(GREEN_LED);
Led *red = new Led(RED_LED);
Sonar *sonar = new Sonar(TRIG_PIN, ECHO_PIN);

void WifiSetup::setup_wifi()
{
    delay(10);

    Serial.println(String("Connecting to ") + ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
void WifiSetup::reconnect()
{

    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");

        // Create a random client ID
        
        String clientId = String("esp-") + String(random(0xffff), HEX);

        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            // client.publish("outTopic", "hello world");
            // ... and resubscribe
            client.subscribe(topic);
            client.subscribe("water-frequency");
        }
        else
        {
            if (green->status())
            {
                green->off();
                red->on();
            }
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void WifiSetup::callback(char *topic, byte *payload, unsigned int length)
{
    if (String(topic) == "water-frequency") {
        int newFrequency = atoi((char*)payload); 
        frequency = newFrequency;
    }
}

void WifiSetup::setupMQTT()
{
    setup_wifi();
    red->on();
    green->off();
    randomSeed(micros());
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void WifiSetup::loopMQTT()
{
    if (!client.connected())
    {
        reconnect();
    }
    else
    {
        green->on();
        red->off();
    }
    client.loop();

    unsigned long now = millis();
    if (now - lastMsgTime > frequency)
    {
        lastMsgTime = now;
        int waterLevel = sonar->getDistance(); 

        snprintf(msg, MSG_BUFFER_SIZE, "%d", waterLevel);

        Serial.println(String("Publishing message: ") + msg);

        /* publishing the msg */
        client.publish(topic, msg);
    }
}
