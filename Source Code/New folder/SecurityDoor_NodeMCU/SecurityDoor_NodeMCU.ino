#define BLYNK_TEMPLATE_ID "TMPL6fW1w5XkQ"
#define BLYNK_TEMPLATE_NAME "PT2"
#define BLYNK_AUTH_TOKEN "nGDRvwhlp19wr9ZbIxAvVN6nu4P8Z9G7"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <SerialCommand.h>
SerialCommand SCmd;
SimpleTimer timer;

char ssid[]="FPTU_Student";
char pass[]="12345678";

void setup() {
  Serial.begin(9600);
  SCmd.addCommand("doorUnlock", doorU);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(200, readValueFromBlynk);
  timer.setInterval(100, doorU);
}

void loop() {
  SCmd.readSerial();

}
void sendValueToBlynk() {
  Blynk.virtualWrite(V0, 1); // Send 1 (true) to V0
  timer.setTimeout(5000, resetValueToBlynk);
}

void doorU() {
  timer.setInterval(1000, sendValueToBlynk);
}
void resetValueToBlynk() {
  Blynk.virtualWrite(V0, 0); // Set to 0 (false) after 5 seconds
}
void resetDoor() {
  Blynk.virtualWrite(V1, 0);
}
void readValueFromBlynk() {
  int value = digitalRead(V1);
  if (value == 1) {
    Serial.println("openDoor");
    value = 0;
  }
  timer.setTimeout(500, resetDoor);
}
