#define BLYNK_PRINT Serial

// Blynk template ID, name, and authentication token

#define BLYNK_TEMPLATE_ID "TMPL6FeMMQqnB"
#define BLYNK_TEMPLATE_NAME "PT2"
#define BLYNK_AUTH_TOKEN "jhQbVa_kmTU_UlSYjrS0tiUUxVxBIWlV"

#include <SoftwareSerial.h>

#include <BlynkMultiClient.h>      // Include the BlynkMultiClient library

#include <ESP8266WiFi.h>           // Include the ESP8266WiFi library
#include <Wire.h>

// Your WiFi credentials.

// Set password to "" for open networks.

const char* ssid = "FPTU_Student";

const char* pass = "12345678";


BlynkTimer timer;                   // Create a Blynk timer object

static WiFiClient blynkWiFiClient;  // Create a WiFi client object for Blynk

void connectWiFi() {

  Serial.print("Connecting to ");

  Serial.println(ssid);

  // Connect to the WiFi network with or without a password

  if (pass && strlen(pass)) {

    WiFi.begin((char*)ssid, (char*)pass);

  } else {

    WiFi.begin((char*)ssid);

  }

  // Wait for WiFi connection

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

}


void setup() {

  // Debug console

  Serial.begin(9600);

  connectWiFi();  // Connect to the WiFi network

  // Setup Blynk

  Blynk.addClient("WiFi", blynkWiFiClient, 80);  // Add Blynk client with name "WiFi"

  Blynk.config(BLYNK_AUTH_TOKEN);                 // Set Blynk authentication token

  pinMode(D7, OUTPUT);  // Set D7 pin as an output
  Wire.begin(D1, D2);
}

void loop() {

  // Reconnect WiFi if not connected

  if (WiFi.status() != WL_CONNECTED) {

    connectWiFi();

    return;

  }

  Blynk.run();   // Run Blynk library

  timer.run();   // Run Blynk timer

}

// Blynk callback for handling writes to virtual pin V2

BLYNK_WRITE(V0) {

  int bn = param.asInt();


  if (bn == 1) {
    Wire.beginTransmission(8); /* begin with device address 8 */
    Wire.write("OpenDoor");  /* sends hello slave string */
    Wire.endTransmission();    /* stop transmitting */
    delay(1000);
    bn = 0;
    Blynk.virtualWrite(V0, 0);
  }

}
