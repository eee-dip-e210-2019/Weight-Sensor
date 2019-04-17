#include "ESP8266HTTPClient.h";
#include "ESP8266WiFi.h";
#include "ArduinoJson.h";
#include "HX711.h";

const int DOUT_PIN=D2;
const int SCK_PIN=D3; //hx711 circuitry
const char *ssid="GiaoGiao!";
const char *password="zhangsiyuan0901"; //router info
long reading;
const char* json;

HX711 scale;
HTTPClient http;

int DAQ() {

    scale.set_offset(-18600);
    scale.set_scale(700);
    reading=scale.get_units(10);

    return reading;

}

void setup() {
 
  Serial.begin(115200); //Serial connection
  
  scale.begin(DOUT_PIN,SCK_PIN);
  
  WiFi.begin(ssid, password); //WiFi connection
  Serial.println("");
 
  while (WiFi.status() != WL_CONNECTED) {  
    //Wait for the WiFI connection completion
 
    delay(1000);
    Serial.println("Connecting...");
 
  }
  Serial.println("");
  Serial.print("Connected to: ");
  Serial.print(ssid);
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());
 
}
 
void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    StaticJsonBuffer<200> jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["id"]="0";

    if (DAQ()>100) {
      
      root["value"]="true";
      
    }

    else {
      
      root["value"]="false";
      
    }

    

    char BS[200];
    root.prettyPrintTo(BS);
 
    http.begin("http://104.248.151.157/api");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(BS);  //Send the request
    String payload = http.getString();  //Get the response payload

    Serial.println("");
    Serial.println("Weight: ");
    Serial.print(reading);  //Print weight detected by load cell
    Serial.println("");
    Serial.println(BS);  //Print Json massage
    Serial.println("");
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println("");
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
 
}
