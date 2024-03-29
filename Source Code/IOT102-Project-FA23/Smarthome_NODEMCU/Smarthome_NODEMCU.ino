#define BLYNK_PRINT Serial

// Blynk template ID, name, and authentication token

#define BLYNK_TEMPLATE_ID "TMPL6pywv26g9"
#define BLYNK_TEMPLATE_NAME "Smarthome"
#define BLYNK_AUTH_TOKEN "kavmAUcpsBJ92dRbWciP2-3vAQWbeWZb"

#include <SoftwareSerial.h>

#include <BlynkMultiClient.h> // Include the BlynkMultiClient library

#include <ESP8266WiFi.h> // Include the ESP8266WiFi library
#include <Wire.h>

// Your WiFi credentials.

// Set password to "" for open networks.

// Khai báo chân kết nối với input
const int gasPin = D3;
const int motionPin = D4;
const int btnThiefPin = D5;

// Khai báo biến khác
boolean antiThiefMode = false;
String thief = "Normal";
String gas = "Normal";

// Kết thúc khai báo
const char *ssid = "ThuyTrang";

const char *pass = "18092005";

BlynkTimer timer; // Create a Blynk timer object

static WiFiClient blynkWiFiClient; // Create a WiFi client object for Blynk

void connectWiFi()
{

  Serial.print("Connecting to ");

  Serial.println(ssid);

  // Connect to the WiFi network with or without a password

  if (pass && strlen(pass))
  {

    WiFi.begin((char *)ssid, (char *)pass);
  }
  else
  {

    WiFi.begin((char *)ssid);
  }

  // Wait for WiFi connection

  while (WiFi.status() != WL_CONNECTED)
  {

    delay(500);

    Serial.print(".");
  }
}

void setup()
{

  // Debug console

  Serial.begin(9600);

  connectWiFi(); // Connect to the WiFi network

  // Setup Blynk

  Blynk.addClient("WiFi", blynkWiFiClient, 80); // Add Blynk client with name "WiFi"

  Blynk.config(BLYNK_AUTH_TOKEN); // Set Blynk authentication token
  // Setup ở đây
  pinMode(gasPin, INPUT);
  pinMode(motionPin, INPUT);
  pinMode(btnThiefPin, INPUT_PULLUP);
  Wire.begin(D1, D2);
}

void loop()
{

  // Reconnect WiFi if not connected

  if (WiFi.status() != WL_CONNECTED)
  {

    connectWiFi();

    return;
  }

  Blynk.run(); // Run Blynk library

  timer.run(); // Run Blynk timer

  // Code bắt đầu ở đây
  checkKeys();
  Blynk.virtualWrite(V1, thief);
  Blynk.virtualWrite(V3, gas);

  if (antiThiefMode == true)
  {
    antiThief();
  }
  gasDetect();
  // Code kết thúc

  delay(5000);
}
void checkKeys()
{
  if (digitalRead(btnThiefPin) == LOW)
  {
    if (antiThiefMode == true)
    {
      antiThiefMode = false;
      Blynk.virtualWrite(V2, 0);
    }
    else
    {
      antiThiefMode = true;
      Blynk.virtualWrite(V2, 1);
    }
  }
}

void alarmState()
{
  thief = "Someone is in your house!";

  // Ghi chuỗi cảnh báo vào chân V1
  Blynk.virtualWrite(V1, thief);

  // Đợi 5 giây trước khi gửi chuỗi "Bình thường"
  timer.setTimeout(5000L, []()
                   {
    thief = "Normal";
    Blynk.virtualWrite(V1, thief); });
}

void antiThief()
{
  if (digitalRead(motionPin) == HIGH)
  {
    Serial.println("Alarm");
    Wire.beginTransmission(8); /* begin with device address 8 */
    Wire.write("Alarm");
    Wire.endTransmission(); /* stop transmitting */
    alarmState();
  }

  delay(5000); // Đợi 5 giây trước khi tiếp tục kiểm tra
}

void gasDetect()
{
  if (digitalRead(gasPin) == HIGH)
  {
    Serial.println("Gas");
    Wire.beginTransmission(8); /* begin with device address 8 */
    Wire.write("Alarm");
    Wire.endTransmission(); /* stop transmitting */
    gas = "Gas is detected!";
    Blynk.virtualWrite(V3, gas);
    timer.setTimeout(5000L, []()
                     {
    gas = "Normal";
    Blynk.virtualWrite(V3, gas); });
  }

  delay(5000); // Đợi 5 giây trước khi tiếp tục kiểm tra
}
BLYNK_WRITE(V2)
{
  int bn = param.asInt();
  if (bn == 1)
  {
    antiThiefMode = true;
  }
  else
  {
    antiThiefMode = false;
  }
}

BLYNK_WRITE(V0)
{

  int bn = param.asInt();

  if (bn == 1)
  {
    Serial.println("OpenDoor");
    Wire.beginTransmission(8); /* begin with device address 8 */
    Wire.write("OpenDoor");
    Wire.endTransmission(); /* stop transmitting */
    delay(5000);
    bn = 0;
    Blynk.virtualWrite(V0, 0);
  }
}
