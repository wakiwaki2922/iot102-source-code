#include<Wire.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#define Rx 11             //Định nghĩa chân 11 là Rx
#define Tx 12             //Định nghĩa chân 12 là Tx

SoftwareSerial mySerial(2, 3); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

Servo doorServo;
const int servoPin = 9;
uint8_t id; // Khai báo biến 'id' ở phạm vi toàn cục
String str = "";

const int btnEnrollPin = 4; // Sử dụng chân số 4 cho nút enroll
const int btnOpenPin = 5;   // Sử dụng chân số 5 cho nút open

void setup()
{
  Serial.begin(9600);
  finger.begin(57600);
  Wire.begin(8);
  Wire.onReceive(receiveEvent); /* register receive event */

  pinMode(btnEnrollPin, INPUT_PULLUP); // Sử dụng chân btnEnrollPin cho nút enroll
  pinMode(btnOpenPin, INPUT_PULLUP);   // Sử dụng chân btnOpenPin cho nút open

  pinMode(servoPin, OUTPUT); // Khai báo chân doorOPin là chân điều khiển servo mở cửa
  while (!Serial) {
    ;
  }
  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor");
    Serial.println("Module not found. Check connections.");
    // while (1)
    //   ;
  }
}

void loop()
{
  Serial.println("Place Finger [ ]");
  int result = getFingerprintIDez();
  if (result > 0)
  {
    tgDoor();
  }
  checkKeys();
  delay(1000);
}

void tgDoor()
{
  Serial.println("Please Wait... Door is Opening");
  delay(1000);

  doorServo.attach(servoPin); // Gắn servo vào chân servoPin
  doorServo.write(90);        // Mở cửa (quay servo 90 độ)

  delay(500); // Chờ một khoảng thời gian

  Serial.println("Welcome");
  Serial.println("Door is Open");

  for (int i = 0; i < 8; i++)
  {
    delay(1000);
  }

  Serial.println("Please Wait... Door is Closing");
  delay(1000);

  doorServo.write(-90); // Đóng cửa (quay servo -90 độ)

  delay(500); // Chờ một khoảng thời gian

  doorServo.detach(); // Tắt servo

  delay(100);
}

void Enroll()
{
  int count = 0;
  Serial.println("Enroll Finger");
  while (1)
  {
    if (digitalRead(btnOpenPin) == 0)
    {
      count++;
      if (count > 128)
        count = 0;
      delay(300);
    }
    else if (digitalRead(btnEnrollPin) == 0)
    {
      id = count;
      getFingerprintEnroll();
      return;
    }
  }
}

uint8_t getFingerprintEnroll()
{
  int p = -1;
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println("No Finger");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  p = finger.image2Tz(1);
  switch (p)
  {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER)
  {
    p = finger.getImage();
  }
  Serial.print("ID ");
  Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        return p;
    }
  }

  p = finger.image2Tz(2);
  switch (p)
  {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  p = finger.createModel();
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Prints matched!");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_ENROLLMISMATCH)
  {
    Serial.println("Fingerprints did not match");
    return p;
  }
  else
  {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID ");
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Stored!");
    return p;
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION)
  {
    Serial.println("Could not store in that location");
    return p;
  }
  else if (p == FINGERPRINT_FLASHERR)
  {
    Serial.println("Error writing to flash");
    return p;
  }
  else
  {
    Serial.println("Unknown error");
    return p;
  }
}

int getFingerprintIDez()
{
  uint8_t p = finger.getImage();

  if (p != FINGERPRINT_OK)
    return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
    Serial.println("Finger Not Found");
    Serial.println("Try Later");
    return -1;
  }
  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  return finger.fingerID;
}

void checkKeys()
{
  if (digitalRead(btnEnrollPin) == 0)
  {
    Serial.println("Please Wait");
    delay(1000);
    while (digitalRead(btnEnrollPin) == 0)
      ;
    Enroll();
  }
  else if (digitalRead(btnOpenPin) == 0)
  {

    Serial.println("Please Wait");
    delay(1000); 
    while (digitalRead(btnOpenPin) == 0)
      ;
    tgDoor();
    str="";
  }else if ( str=="OpenDoor")
  {
    str="";
    Serial.println("Please Wait");
    delay(1000); 
    while (digitalRead(btnOpenPin) == 0)
      ;
    tgDoor();
  }
}
void receiveEvent(int howMany) {

  while (0 < Wire.available()) {
    char c = Wire.read();    /* Nhận byte như một ký tự */
    str += c;               /* Nối ký tự vào chuỗi str */
  }
  Serial.println(str);
}
