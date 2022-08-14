// code to be run on the ESP32 module
#include "Led.h"
#include "Photoresistor.h"
#include "TemperatureSensor.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define MSG_BUFFER_SIZE  50
#define MSG_TIME 10000

// sensor pins
#define PIN_TEMP 34
#define PIN_LED 26
#define PIN_PHOTO 32

/* WiFi Ryan*/
/*
const char* ssid = "Mi10";
const char* password = "12345678";
*/

/* WiFi HDavi casa */
const char* ssid = "TIM-90212215";
const char* password = "4s07Twc1U7Iu7CfTJ4a7wXGq";

/* WiFi HDavi telefono */
/*
const char* ssid = "HDavi";
const char* password = "Thegreatd12!";
*/

/* MQTT server address */
const char* mqtt_server = "broker.mqtt-dashboard.com";

/* MQTT client management */
WiFiClient espClient;
PubSubClient client(espClient);

/* Sensors */
TemperatureSensor *temp;
Photoresistor *photo;
Led *led;

unsigned long lastMsgTime = 0;
char msg[MSG_BUFFER_SIZE];

void setup_wifi() {
  delay(10);
  Serial.println(String("Connecting to ") + ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/* MQTT subscribing callback */
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println(String("Message arrived on [") + topic + "] len: " + length );
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Create a random client ID
    String clientId = String("esiot-2122-client-")+String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("esiot-2122");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  randomSeed(micros());
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  temp = new TemperatureSensor(PIN_TEMP);
  photo = new Photoresistor(PIN_PHOTO);
  led = new Led(PIN_LED);
  led -> switchOn();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsgTime > MSG_TIME) {
    lastMsgTime = now;

    /* getting values from sensors */
    int luminosity = photo -> getLuminosity();
    int temperature = temp -> getTemperature();
    /* creating a msg in the buffer */
    snprintf (msg, MSG_BUFFER_SIZE, "L: %d - T: %d", luminosity, temperature);

    Serial.println(String("Publishing message: ") + msg);
    
    /* publishing the msg */
    client.publish("esiot-2122", msg);  
  }
}
