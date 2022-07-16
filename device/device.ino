#include <WiFi.h>
#include <ArduinoJson.h>
#include <binary.h>
#include <PubSubClient.h>

#include <HTTPClient.h>

#define WIFI_STA_NAME "TELECOM08_2.4G"

#define WIFI_STA_PASSWORD "162404710008"

#define MQTT_SERVER "m16.cloudmqtt.com"
#define MQTT_PORT 16319
#define MQTT_USER_ID "GESTURE01"
#define MQTT_USERNAME "ggaomyqh"
#define MQTT_PASSWORD "3wjA27NFU3ET"

float calibration_value = 21.34 + 5.1;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;
float ph_act;

void setup()
{

    lcd.begin();
    lcd.backlight();
    sensors.begin();

    Serial.begin(115200);
    Serial.setDebugOutput(true);

    pinMode(LED_BUILTIN, OUTPUT);

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_STA_NAME);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }

    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    mqtt.setServer(MQTT_SERVER, MQTT_PORT);

    Serial.println(WIFI_SSID);

    WiFi.setAutoReconnect(true);

    sensors.begin();

    init_http.DEBUG_MODE = true;
}

void loop()
{
    if (mqtt.connected() == false)
    {
        Serial.print("MQTT connection... ");
        if (mqtt.connect(MQTT_USER_ID, MQTT_USERNAME, MQTT_PASSWORD))
        {
            Serial.println("connected");
            sendSensorData();
        }
        else
        {
            Serial.println("failed");
            delay(5000);
        }
    }
    else
    {
        mqtt.loop();
        // .
        // .
        // .
        sendSensorData()
        delay(100)
    }
}

void sendSensorData()
{
  StaticJsonDocument<1024> doc;

  doc["id"] = "AN7jxxIlMGhcst0vaFak";
  doc["temp"] = 26;
  doc["pH"] = 8;

  serializeJson(doc, output);

  mqtt.publish("/sensors", output);
}