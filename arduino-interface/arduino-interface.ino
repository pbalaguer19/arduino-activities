#include  <Wire.h>                // I2C access
#include  <LiquidCrystal_I2C.h>   // LCD library
#define LCD_LEN 16
#define SLAVE_ADDRESS 0x04

float base_pitch = 0;
float base_roll = -5;

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
  read_bcg();

  delay(200);
  
}

void read_bcg(){
  boolean not_finished = true;
  String info;
  
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
  print_lcd(info);
  message = info + "%";
}

void send_data_to_raspberry(){
  char buffer[32];
  message.toCharArray(buffer, 32);
  Wire.write(buffer); 
  
}

void print_lcd(String message){
  double pitch = getValue(message, '/', 0).toDouble();
  double roll = getValueBetween(message, '/', ' ').toDouble();  
  String movement = getMovement(pitch, roll);
  
  int acceleration = getValue(message ,' ',1).toInt();

  lcd.clear();

  lcd.setCursor ( 0, 0 );
  lcd.print("M: " + movement);

  lcd.setCursor ( 0, 1 );
  lcd.print("Acc: " + String(acceleration));

  Serial.println(movement + " " + String(acceleration));
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index){
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

String getValueBetween(String data, char separator1, char separator2){
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex; i++){
    if(data.charAt(i)==separator1){
        strIndex[0] = i + 1;
    }
    if(data.charAt(i)==separator2){
        strIndex[1] = i + 1;
    }
  }

  return data.substring(strIndex[0], strIndex[1]);
}

String getMovement(double pitch, double roll){
  if (abs(roll - base_roll) > abs(pitch - base_pitch)) {
    if (roll < base_roll - 5) return "DOWN";
    if (roll > base_roll + 5) return "UP";
  }
  else {
    if (pitch < base_pitch - 5) return "RIGHT";
    if (pitch > base_pitch + 5) return "LEFT";
  }

  return "";
}
