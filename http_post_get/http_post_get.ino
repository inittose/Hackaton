#include <HTTPClient.h>
#include <Dictionary.h>
#include "esp_wifi.h"

const char* ssid = "TP-Link_4F90";
const char* password =  "00608268";
const char* posts = "http://jsonplaceholder.typicode.com/posts";
const uint8_t DATA_NUM = 7;
Dictionary &data = *(new Dictionary(DATA_NUM));

String GetJsonParameter(const String & key);

void setup() {
  
  Serial.begin(115200);
  delay(4000);   //Delay needed before calling the WiFi.begin
  
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("Connected to the WiFi network");
  
}
  
void loop() {
  
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
  
   HTTPClient http;   
  
   http.begin(posts);  //Specify destination for HTTP request
   http.addHeader("Content-Type", "text/plain");             //Specify content-type header
  
   int httpResponseCode = http.POST("POSTING from ESP32");   //Send the actual POST request
  
   if(httpResponseCode>0){
  
    String response = http.getString();                       //Get the response to the request
  
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
  
   }else{
  
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  
   }
  
   http.end();  //Free resources
  
 }else{
  
    Serial.println("Error in WiFi connection");   
  
 }
  
  delay(10000);  //Send a request every 10 seconds
  
}

String GetJsonParameter(const String & key)
{
  String json;
  json += key + " : " + data[key];
  return json;
}