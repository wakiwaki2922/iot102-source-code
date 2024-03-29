#define BLYNK_TEMPLATE_ID "TMPL6FeMMQqnB"
#define BLYNK_TEMPLATE_NAME "PT2"
#define BLYNK_AUTH_TOKEN "jhQbVa_kmTU_UlSYjrS0tiUUxVxBIWlV"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
//#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
//#include <SerialCommand.h>
//SerialCommand SCmd;
BlynkTimer timer;
int value;
char ssid[] = "FPTU_Student";
char pass[] = "12345678";

BLYNK_WRITE(V0) {
  value = param.asInt();
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

}

void loop() {
  Blynk.run();
  timer.run();
  if (value == 1) {
    Serial.println("OpenDoor");
    delay(5000);
    value = 0;
    Blynk.virtualWrite(V0, 0);
  }
}
