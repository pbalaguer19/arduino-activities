#include  <Wire.h>                // I2C access
#include  <LiquidCrystal_I2C.h>   // LCD library
#define LCD_LEN 16

/*
* Group Number 4:
* Group Members: Pau Balaguer, Didac Florensa and Hongzhi Zhu
*/

struct __attribute__ ((packed)) controller {
  int32_t x;
  int32_t y;
  int32_t z;
  int32_t acc;
  int32_t hr;
};

// LCD 16x2 on I2C 0x27 address
LiquidCrystal_I2C lcd(0x27, 16, 2);
struct controller movement;

void setup(){
  Serial.begin(9600);
  
  lcd.init();
  lcd.home ();
  
  delay(500);
}

void loop(){
  read_accelerometer();
  print_lcd_distance();
  
  delay(1000);
}

void read_accelerometer(){
  if (Serial.available()){
    Serial.readBytes((char*)&movement, sizeof(controller));
  }
}

void print_lcd_distance(){
  lcd.clear();
  
  lcd.setCursor ( 0, 0 );
  lcd.print(String(movement.x));
  lcd.setCursor ( 0, 1 );
  lcd.print("AA");
}
