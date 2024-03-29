//#include <common.h>
#include <FirebaseESP8266.h>
#include <FirebaseFS.h>
//#include <Utils.h>
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "skilful-firefly-346314-default-rtdb.asia-southeast1.firebasedatabase.app" // *
#define FIREBASE_AUTH "kZSC36qoGZMniB1g0tAlgnzj1t9fC7uq8tgVJx5l" //*
const String ssid = "ThuyTrang"; //*
const String pass = "18092005";

FirebaseData firebaseData;
float t = 30.56;
void setup() {

  Serial.begin(115200);
  Serial.println("");
  Serial.print("Connect to WiFi....");
  Serial.println(ssid);
  delay(1000);
  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address : "); 
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.setFloat(firebaseData, "/temperature: ", t))
  {
    Serial.println("PASSED");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println();
  }
}
