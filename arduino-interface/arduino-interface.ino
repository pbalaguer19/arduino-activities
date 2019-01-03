#include  <Wire.h>                // I2C access
#include  <LiquidCrystal_I2C.h>   // LCD library
#define LCD_LEN 16
#define SLAVE_ADDRESS 0x04

/*
* Group Number 4:
* Group Members: Pau Balaguer, Didac Florensa and Hongzhi Zhu
* Exercise 2.3.receiver
*/

// LCD 16x2 on I2C 0x27 address
LiquidCrystal_I2C lcd(0x27, 16, 2);
String message;

void setup(){
  Serial.begin(9600);
  
  lcd.init();
  lcd.home ();

  pinMode(13, OUTPUT);
 
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onRequest(send_data_to_raspberry);

  delay(500);
}

void loop(){
  read_accelerometer();
  send_data_to_raspberry();

  delay(200);
  
}

void read_accelerometer(){
  boolean not_finished = true;
  String info;
  message = "";
  
  while(not_finished){
    if (Serial.available()){
      char car = Serial.read();
      if(car == '%') {
        not_finished = false;
      }
      else {
        info += car;
      }
    }
  }

  message = info + "%";
}

void send_data_to_raspberry(){
  char buffer[32];
  message.toCharArray(buffer, 32);
  Wire.write(buffer);
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
/*String getValue(String data, char separator, int index)
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
}*/
