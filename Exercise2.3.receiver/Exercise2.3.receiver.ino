#include  <Wire.h>                // I2C access
#include  <LiquidCrystal_I2C.h>   // LCD library
#define LCD_LEN 16

/*
* Group Number 4:
* Group Members: Pau Balaguer, Didac Florensa and Hongzhi Zhu
* Exercise 2.1
*/

// LCD 16x2 on I2C 0x27 address
LiquidCrystal_I2C lcd(0x27, 16, 2);

//MIN and MAX variables
int min_cm, max_cm, proportion;
long cm;

void setup(){
  Serial.begin(9600);
  
  lcd.init();
  lcd.home ();
  
  delay(500);

  //read_min_max_values();
}

void loop(){
  read_sensor_distance();
  
  if( cm >= min_cm && cm <= max_cm) print_lcd_distance();
  
  delay(1000);
  
}

void read_min_max_values(){
  min_cm = read_serial_integer();
  max_cm = read_serial_integer();

  if (max_cm <= min_cm) max_cm = min_cm + 2;
  proportion = (max_cm - min_cm) / LCD_LEN;

  // Send it to Emissor Arduino
  Serial.print(String(min_cm) + "\n");
  Serial.print(String(max_cm) + "\n");
}

int read_serial_integer(){
  boolean int_not_finished = true;
  int number = 0;

  while(int_not_finished){
    if (Serial.available()){
      String car = Serial.readString();
      Serial.println("Receiving dates: ");
      Serial.println(car);
      int_not_finished = false;
    }
  }

  return number;
}

void read_sensor_distance(){
  cm = read_serial_integer();
}

void print_lcd_distance(){
  //Print "progress bar"
  int progress_val = cm - min_cm;
  String progress = "";

  // Turn off led
  digitalWrite(LED_BUILTIN, LOW);
  
  while (progress_val > 0){
    progress += "X";
    progress_val -= proportion;
  }  

  lcd.clear();
  lcd.setCursor ( 0, 0 );
  lcd.print(progress);
  lcd.setCursor ( 0, 1 );
  lcd.print("Dist: " + String(cm) + " cm" );
}

void open_led(){
  lcd.clear();
  
  // Turn on led
  digitalWrite(LED_BUILTIN, HIGH);
}
