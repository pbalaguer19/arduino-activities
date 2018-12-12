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
int acceleration;

void setup(){
  Serial.begin(9600);
  
  lcd.init();
  lcd.home ();

  delay(500);
}

void loop(){
  read_accelerometer();
  print_lcd_distance();

  delay(200);
  
}

void read_accelerometer(){
  boolean not_finished = true;
  String info;
  
  movement = "";
  acceleration = 0;
  
  while(not_finished){
    if (Serial.available()){
      char car = Serial.read();
      if(car == '.') {
        not_finished = false;
      }
      else {
        info += car;
      }
    }
  }

  movement = getValue(info,'-',0);
  acceleration = getValue(info,'-',1).toInt();;
}

void print_lcd_distance(){
  lcd.clear();
  
  lcd.setCursor ( 0, 0 );
  lcd.print("Movement: " + movement);

  lcd.setCursor ( 0, 1 );
  lcd.print("Acc: " + String(acceleration));
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}