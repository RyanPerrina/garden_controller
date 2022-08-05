// code to be run on the ESP32 module
#include "Led.h"
#include "Photoresistor.h"
#include "TemperatureSensor.h"
#include <HTTPClient.h>

// sensor pins
#define PIN_TEMP 34
#define PIN_LED 35
#define PIN_PHOTO 32

const char* ssid = "Mi10";
const char* password = "12345678";
const char* serviceURI = "";
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
   http.addHeader("Content-Type", "application/json");    
    
   String msg = 
    String("{ \"value\": ") + String(temp) + 
    ", \"place\": \"" + place +"\" }";
   
   int retCode = http.POST(msg);
   
   http.end();  
      
   return retCode;
}

void send(int t, int l) {
  if (WiFi.status()== WL_CONNECTED){      

    int code = sendData(serviceURI, t ,l , "home");
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
  //temp = new TemperatureSensor(PIN_TEMP);
  //led = new Led(PIN_LED);
  //photo = new Photoresistor(PIN_PHOTO);
  Serial.println("CIAO BRO PRIMA");
  connectToWifi(ssid, password);
  Serial.println("CIAO BRO DOPO");
}

void loop() {
  delay(10000);
  //send(temp -> getTemperature(), photo -> getLight());
  if(WiFi.status()== WL_CONNECTED){      
    HTTPClient http;
  
    // Your Domain name with URL path or IP address with path
    http.begin(serverPath);
      
    // Send HTTP GET request
    int httpResponseCode = http.GET();
      
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    
    // Free resources
    http.end();

    delay(2000);

  } else {
    Serial.println("WiFi Disconnected... Reconnect.");
    connectToWifi(ssid, password);
  }
}
