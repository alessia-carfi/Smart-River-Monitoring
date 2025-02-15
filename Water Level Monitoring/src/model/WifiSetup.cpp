#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "model/WifiSetup.h"
#include "model/Led.h"
#include "utils/Global.h"

const char *ssid = "";
const char *password = "";

/* MQTT server address for now I keep this */
const char *mqtt_server = "broker.mqtt-dashboard.com";

/* MQTT topic idk what a topic is */
const char *topic = "esiot-2023";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsgTime = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;
Led *green = new Led(GREEN_LED);
Led *red = new Led(RED_LED);

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
        String clientId = String("esiot-2122-client-") + String(random(0xffff), HEX);

        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            // client.publish("outTopic", "hello world");
            // ... and resubscribe
            client.subscribe(topic);
        }
        else
        {
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
    Serial.println(String("Message arrived on [") + topic + "] len: " + length);
}

void WifiSetup::setupMQTT()
{
    Serial.begin(9600);
    setup_wifi();
    randomSeed(micros());
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void WifiSetup::loopMQTT()
{
    if (!client.connected())
    {
        if (green->status())
        {
            green->off();
            red->on();
        }
        reconnect();
    }
    else
    {
        green->on();
        red->off();
    }
    client.loop();

    unsigned long now = millis();
    if (now - lastMsgTime > 10000)
    {
        lastMsgTime = now;
        value++;

        /* creating a msg in the buffer */
        snprintf(msg, MSG_BUFFER_SIZE, "hello world #%ld", value);

        Serial.println(String("Publishing message: ") + msg);

        /* publishing the msg */
        client.publish(topic, msg);
    }
}