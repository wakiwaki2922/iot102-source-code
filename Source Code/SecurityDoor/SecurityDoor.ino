#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial mySerial(2, 3); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal_I2C lcd(0x27, 16, 2);

uint8_t id;
int set_time = 8;

Servo doorServo;
const int servoPin = 9;

const int btnEnrollPin = 4; // Sử dụng chân số 4 cho nút enroll
const int btnOpenPin = 5;   // Sử dụng chân số 5 cho nút open

void setup()
{
  Serial.begin(9600);
  finger.begin(57600);

  pinMode(btnEnrollPin, INPUT_PULLUP); // Sử dụng chân btnEnrollPin cho nút enroll
  pinMode(btnOpenPin, INPUT_PULLUP);   // Sử dụng chân btnOpenPin cho nút open

  pinMode(servoPin, OUTPUT); // Khai báo chân doorOPin là chân điều khiển servo mở cửa

  lcd.init();      // Khởi tạo màn hình LCD
  lcd.backlight(); // Bật đèn nền màn hình LCD
  lcd.setCursor(0, 0);
  lcd.print("Security System");
  lcd.setCursor(0, 1);
  lcd.print("by Finger Print");
  delay(1000); // Đợi một chút
  lcd.clear();
  lcd.print("Finding Module");
  lcd.setCursor(0, 1);
  delay(1000);
  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
    lcd.clear();
    lcd.print("Found Module ");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor");
    lcd.clear();
    lcd.print("module not Found");
    lcd.setCursor(0, 1);
    lcd.print("Check Connections");
  }

  lcd.clear();
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print("Place Finger [ ]");
  lcd.setCursor(0, 1);
  lcd.print("                ");
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
  lcd.clear();
  lcd.print("Please Wait... ");
  lcd.setCursor(0, 1);
  lcd.print(" Door is Opening");
  delay(1000);

  doorServo.attach(servoPin); // Gắn servo vào chân servoPin
  doorServo.write(90);        // Mở cửa (quay servo 90 độ)

  delay(500); // Chờ một khoảng thời gian

  lcd.clear();
  lcd.print("    Welcome   ");
  lcd.setCursor(0, 1);
  lcd.print("  Door is Open ");

  for (int i = 0; i < set_time; i++)
  {
    delay(1000);
  }

  lcd.clear();
  lcd.print("Please Wait... ");
  lcd.setCursor(0, 1);
  lcd.print(" Door is Closing");
  delay(1000);

  doorServo.write(-90); // Đóng cửa (quay servo -90 độ)

  delay(500); // Chờ một khoảng thời gian

  doorServo.detach(); // Tắt servo

  delay(100);
  lcd.clear();
}

void Enroll()
{
  int count = 0;
  lcd.clear();
  lcd.print("Enroll Finger    ");
  lcd.setCursor(0, 1);
  lcd.print("Location:");
  while (1)
  {
    lcd.setCursor(9, 1);
    lcd.print(count);
    lcd.print("   ");

    if (digitalRead(btnOpenPin) == 0)
    {
      count++;
      if (count > 128)
        count = 0;
      delay(300);
    }

    //    else if (digitalRead(btnDownPin) == 1)
    //    {
    //      count--;
    //      if (count < 0)
    //        count = 128;
    //    }

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
  lcd.clear();
  lcd.print("finger ID:");
  lcd.print(id);
  lcd.setCursor(0, 1);
  lcd.print("Place Finger");
  delay(2000);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.clear();
      lcd.print("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No Finger");
      lcd.clear();
      lcd.print("No Finger");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.print("Comm Error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      lcd.clear();
      lcd.print("Imaging Error");
      break;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.print("Unknown Error");
      break;
    }
  }
  // OK success!
  p = finger.image2Tz(1);
  switch (p)
  {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    lcd.clear();
    lcd.print("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    lcd.clear();
    lcd.print("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    lcd.clear();
    lcd.print("Comm Error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    lcd.clear();
    lcd.print("Feature Not Found");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    lcd.clear();
    lcd.print("Feature Not Found");
    return p;
  default:
    Serial.println("Unknown error");
    lcd.clear();
    lcd.print("Unknown Error");
    return p;
  }

  Serial.println("Remove finger");
  lcd.clear();
  lcd.print("Remove Finger");
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
  lcd.clear();
  lcd.print("Place Finger");
  lcd.setCursor(0, 1);
  lcd.print("   Again");
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
      return;
    }
  }
  // OK success!
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

  // OK converted!
  Serial.print("Creating model for #");
  Serial.println(id);

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
    lcd.clear();
    lcd.print("Stored!");
    delay(2000);
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
    lcd.clear();
    lcd.print("Finger Not Found");
    lcd.setCursor(0, 1);
    lcd.print("Try Later ");
    delay(2000);
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
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    while (digitalRead(btnEnrollPin) == 0)
      ;
    Enroll();
  }
  else if (digitalRead(btnOpenPin) == 0)
  {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    while (digitalRead(btnOpenPin) == 0)
      ;
    tgDoor();
  }
}
