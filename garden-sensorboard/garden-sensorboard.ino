// code to be run on the ESP32 module
#include "Led.h"
#include "Photoresistor.h"
#include "TemperatureSensor.h"
#include <HTTPClient.h>
#include <WiFi.h>

// sensor pins
#define PIN_TEMP 34
#define PIN_LED 35
#define PIN_PHOTO 32

/* WiFi Ryan*/
/*
const char* ssid = "Mi10";
const char* password = "12345678";
*/
/* WiFi HDavi casa */
/*
const char* ssid = "TIM-90212215";
const char* password = "4s07Twc1U7Iu7CfTJ4a7wXGq";
*/
/* WiFi HDavi telefono */
const char* ssid = "HDavi";
const char* password = "Thegreatd12!";
const char* serviceURI = "http://3a0b-5-90-198-245.eu.ngrok.io";

TemperatureSensor *temp;
Led *led;
Photoresistor *photo;

void connectToWifi(const char* ssid, const char* password){
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

int sendData(String address, float temp, float light, String place){  
  
   HTTPClient http;    
   http.begin(address + "/api/data");
   //http.addHeader("Content-Type", "application/json");    
    
   String msg = 
    String("{ \"temp\": ") + String(temp) + " \"light\": "
    + String(light) + ", \"place\": \"" + place +"\" }";
   
   int retCode = http.POST(msg);
   http.end();
   return retCode;
}

void send(float t, float l) {
  if (WiFi.status()== WL_CONNECTED){      

    int code = sendData(serviceURI, t, l, "home");
    if (code == 200){
       Serial.println("Ok");   
     } else {
       Serial.println(String("Error: ") + code);
     }
    
    delay(5000);

  } else {
    Serial.println("WiFi Disconnected... Reconnect.");
    connectToWifi(ssid, password);
  }
}

void setup() {
  Serial.begin(115200);
  //temp = new TemperatureSensor(PIN_TEMP);
  //led = new Led(PIN_LED);
  //photo = new Photoresistor(PIN_PHOTO);
  connectToWifi(ssid, password);
}

void loop() {
  //send(temp -> getTemperature(), photo -> getLight());
  send(0, 0);
}
