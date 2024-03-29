/*
   Connect
        Arduino       I2C_LCD
          5V            VCC
          GND           GND
          SDA            SDA
          SCL            SCL
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);


void setup()
{
  lcd.init();                      // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(6, 0);
  lcd.print("Hshop.vn");
  lcd.setCursor(4, 1);
  lcd.print("Hello world!"); \
  lcd.setCursor(6, 2);
  lcd.print("Hshop.vn");
  lcd.setCursor(4, 3);
  lcd.print("Hello world!");

}


void loop()
{
}
