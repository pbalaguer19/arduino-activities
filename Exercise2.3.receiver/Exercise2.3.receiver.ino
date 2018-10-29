#include  <Wire.h>                // I2C access
#include  <LiquidCrystal_I2C.h>   // LCD library
#define LCD_LEN 16

/*
* Group Number 4:
* Group Members: Pau Balaguer, Didac Florensa and Hongzhi Zhu
* Exercise 2.3.receiver
*/

// LCD 16x2 on I2C 0x27 address
LiquidCrystal_I2C lcd(0x27, 16, 2);
String movement;

void setup(){
  Serial.begin(9600);
  
  lcd.init();
  lcd.home ();
  
  delay(500);
}

void loop(){
  read_accelerometer();
  print_lcd_distance();
  
  delay(2000);
  
}

void read_accelerometer(){
  if (Serial.available()) movement = Serial.readString();
}

void print_lcd_distance(){
  lcd.clear();
  
  lcd.setCursor ( 0, 0 );
  lcd.print(movement);
}
