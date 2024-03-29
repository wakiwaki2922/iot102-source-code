#define BLYNK_TEMPLATE_ID "TMPL6FeMMQqnB"
#define BLYNK_TEMPLATE_NAME "PT2"
#define BLYNK_AUTH_TOKEN "jhQbVa_kmTU_UlSYjrS0tiUUxVxBIWlV"
#define BLYNK_FIRMWARE_VERSION        "1.3.2"
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial

char ssid[] = "FPTU_Student";
char pass[] = "12345678";

void setup()
{
  Serial.begin(115200);
  delay(100);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

BLYNK_WRITE(V0)
{
  int pinLED = param.asInt();
    if (pinLED == 1) {
    Serial.println("OpenDoor");
    delay(5000);
    pinLED = 0;
    Blynk.virtualWrite(V0, 0);
  }
}


void loop() {

}
