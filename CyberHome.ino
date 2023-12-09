#include <HTTPClient.h>
#include <Dictionary.h>
#include "esp_wifi.h"
#include <BH1750.h> 
#include <Wire.h> 

// HTTP POST/GET
const char* ssid = "TP-Link_4F90";
const char* password =  "00608268";
const char* serverName = "http://192.168.0.87:5000/AddTemperature";
const uint8_t DATA_NUM = 7;
Dictionary &data = *(new Dictionary(DATA_NUM));

String GetJsonParameter(const String & key);
void PostParameter(const String & key);

// TASK 2
BH1750 lightMeter; 

unsigned long timerr = 0;



void setup() {
  
  Serial.begin(9600);
  delay(4000);   //Delay needed before calling the WiFi.begin
  
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("Connected to the WiFi network");
  
  Wire.begin(); 
  lightMeter.begin(); 
  //ledcSetup(23, 200, 8);  
  //ledcAttachPin(23, 0); 
  pinMode(5, OUTPUT); 
  Serial.println(F("BH1750 Test begin")); 
}
  
void loop() {
  if(millis() - timerr > 10) 
  {  
    PostParameter("lux");
  
    float lux = lightMeter.readLightLevel(); 
    int light = (map(lux, 0, 300, 256, 0)); 
    light = constrain(light, 0, 256); 
    analogWrite(5, light); 
    data("lux", lux);
    Serial.println(light); 
    timerr = millis();
  } 
  
}

String GetJsonParameter(const String & key)
{
  String json;
  json += "{\"value\": " + data[key] + "}";
  return json;
}

void PostParameter(const String & key)
{
  if(WiFi.status()== WL_CONNECTED)
    {   //Check WiFi connection status
      HTTPClient http;   
  
      http.begin(serverName);  //Specify destination for HTTP request
      http.addHeader("Content-Type", "text/plain");             //Specify content-type header
  
      int httpResponseCode = http.POST(GetJsonParameter("lux"));   //Send the actual POST request
  
      if(httpResponseCode>0){
  
        String response = http.getString();                       //Get the response to the request
  
        Serial.println(httpResponseCode);   //Print return code
        Serial.println(response);           //Print request answer
  
      }else{
  
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
  
      }
  
      http.end();  //Free resources
  
    }
    else
    {
        Serial.println("Error in WiFi connection");  
    } 
}