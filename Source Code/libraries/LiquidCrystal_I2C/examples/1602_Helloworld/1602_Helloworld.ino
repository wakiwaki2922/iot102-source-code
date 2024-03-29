/*
 * Connect
 *      Arduino       I2C_LCD
 *        5V            VCC
 *        GND           GND
 *        A4            SDA
 *        A5            SCL
 */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);


void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("Hshop.vn");
  lcd.setCursor(2,1);
  lcd.print("Hello world!");
}


void loop()
{
}
