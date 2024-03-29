#include <ESP8266WiFi.h>

const String ssid = "ThuyTrang";
const String pass = "18092005";

void setup() {
 Serial.begin(115200);
  Serial.println("");
  Serial.print("Connect to wifi .... ");
  Serial.println(ssid); 
  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
}
