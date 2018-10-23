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

// X Y Z default values
int x = 0, previous_x = 0;
int y = 0, previous_y = 0;
int z = 0, previous_z = 0;

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
  previous_x = x;
  previous_y = y;
  previous_z = z;
  
  x = read_coord();
  y = read_coord();
  z = read_coord();
}

int read_coord(){
  if (Serial.available()){
    String car = Serial.readStringUntil(" ");
    return car.toInt();
  }
  return 0;
}

void print_lcd_distance(){
  lcd.clear();
  
  lcd.setCursor ( 0, 0 );
  lcd.print(String(x) + " " + String(y) + " " + String(z));
  
  lcd.setCursor ( 0, 1 );
  String mvnt = get_movement();
  lcd.print(mvnt);
}

String get_movement(){
  if(x > previous_x) return "RIGHT";
  if(x < previous_x) return "LEFT";
  if(z > previous_z) return "UP";
  if(z < previous_z) return "RIGHT";
  
}
