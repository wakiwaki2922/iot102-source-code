#include <Wire.h>

void setup() {
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  Serial.begin(9600);           /* start serial for debug */
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
  String str = "";
  while (0 < Wire.available()) {
    char c = Wire.read();    /* Nhận byte như một ký tự */
    str += c;               /* Nối ký tự vào chuỗi str */
  }
  Serial.println(str);          /* to newline */
}
