#include <Keypad.h>
#include <EEPROM.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Ultrasonic.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int address = 0;
const int maxDigits = 5;
char inputDigits[maxDigits + 1];
int digitCount = 0;
int passInput = 0;
int passcurrent = 0;
Servo myservo;
const int servoPin = 12;

const int triggerPin = 11;
const int echoPin = 13;

const int thresholdDistance = 50;
#define photoresistor A3
#define led A2

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 10, 9, 8, 7 };
byte colPins[COLS] = { 6, 5, 4, 3 };

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

const int buzzerPin = 2;

void setup() {
  EEPROM.begin();
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(led, OUTPUT);
  myservo.attach(servoPin);
  lcd.init();       // Khởi tạo màn hình LCD
  lcd.backlight();  // Bật đèn nền màn hình LCD
  lcd.setCursor(0, 0);
}

void loop() {
  openLed();
  loadPassword();
  // Đo khoảng cách bằng cảm biến siêu âm
  long duration;
  int distance;

  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  // Tính khoảng cách dựa trên thời gian
  distance = duration * 0.034 / 2;  // Tính khoảng cách trong đơn vị cm

  // Kiểm tra nếu khoảng cách nhỏ hơn ngưỡng
  if (distance < thresholdDistance) {
  if (passcurrent == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No password");
    lcd.setCursor(0, 1);
    lcd.print("Create one");
    inputPassword();
    savePassword();
  } else {
    int countErr = 0;
    do {
      lcd.clear();
      lcd.print("Enter password:");
      inputPassword();
      if (passcurrent == passInput) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Log in success");
        toggleDoor();
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Want change password(A):");
        for (int i = 0; i < 100; i++) {
          char confirmChange = customKeypad.getKey();
          if (confirmChange == 'A') {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Enter pass change");
            inputPassword();
            savePassword();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Change success");
            delay(2000);
          }
          delay(50);
        }
        break;
      } else {
        countErr++;
        if (countErr < 3) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Wrong pass");
          delay(2000);
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Wrong 3 times");
          digitalWrite(buzzerPin, HIGH);
          delay(10000);
          digitalWrite(buzzerPin, LOW);
          break;
        }
      }
    } while (countErr < 3);
  }
  }
}

void inputPassword() {
  while (true) {
    char customKey = customKeypad.getKey();

    if (customKey >= '0' && customKey <= '9') {
      if (digitCount < maxDigits) {
        inputDigits[digitCount] = customKey;
        lcd.setCursor(digitCount, 1);
        lcd.print(customKey);
        digitCount++;
      }
    }

    if (digitCount == maxDigits) {
      inputDigits[digitCount] = '\0';
      passInput = atoi(inputDigits);
      digitCount = 0;
      break;
    }
  }
}

void loadPassword() {
  passcurrent = 0;
  for (int i = 0; i < 5; i++) {
    byte value = EEPROM.read(address + i);
    passcurrent = (passcurrent * 10) + value;
  }
}

void savePassword() {
  int value = passInput;
  for (int i = 4; i >= 0; i--) {
    EEPROM.write(i, value % 10);
    value = (value - value % 10) / 10;
  }
}

void openLed() {
  int sensorValue = analogRead(photoresistor);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  Serial.println(voltage);
  if (voltage < 1) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}

void toggleDoor() {
  lcd.setCursor(0, 1);
  lcd.print("Door is opened");
  myservo.write(90);  // Mở cửa (quay servo đến 90 độ)
  delay(5000);        // Đợi 5 giây
  lcd.setCursor(0, 1);
  lcd.print("Door is closed");
  myservo.write(-90);  // Đóng cửa (quay servo đến -90 độ)
  delay(2000);         // Đợi 2 giây
}

bool checkPassword(const char *input) {
  // So sánh chuỗi nhập với mật khẩu đúng
  if (strstr(input, passcurrent) != nullptr) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Password correct");
    delay(2000);
    lcd.clear();
    return true;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Password incorrect");
    delay(2000);
    lcd.clear();
    return false;
  }
}
