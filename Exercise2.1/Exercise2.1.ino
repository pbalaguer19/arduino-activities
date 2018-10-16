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

// ULTRA SOUND SENSOR Settings
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo
long duration, cm;


//MIN and MAX variables
int min_cm, max_cm, proportion;

void setup(){
  Serial.begin(9600);

  //USS pinMode
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //LED pinMode
  pinMode(LED_BUILTIN, OUTPUT);
  
  lcd.init();
  lcd.home ();
  
  delay(500);

  read_min_max_values();
}

void loop(){
  read_sensor_distance();
  
  if( cm >= min_cm && cm <= max_cm) print_lcd_distance();
  else open_led();
  
  delay(1000);
  
}

void read_min_max_values(){
  Serial.println ("Enter the MIN distance value (cm)");
  min_cm = read_serial_integer();

  Serial.println ("Enter the MAX distance value (cm)");
  max_cm = read_serial_integer();

  if (max_cm <= min_cm){
    Serial.println ("MAX value cannot be lower or equal than MIN value");
    max_cm = min_cm + 2;
    Serial.println ("MAX value switched to " + String(max_cm));
  }

  proportion = (max_cm - min_cm) / LCD_LEN;
  Serial.println ("Min: " + String(min_cm) + " Max: " + String(max_cm));
}

int read_serial_integer(){
  boolean int_not_finished = true;
  int number = 0;

  while(int_not_finished){
    if (Serial.available()){
      char car = Serial.read();
      switch (car)
      {
        case '\n': { 
          // Endchar. User finished the number.
          if(number > 0) int_not_finished = false;
        }
        default: {
          // User is writing a number. 
          // Move to the right the current number and append the new one.
          if ((car >= '0') && (car <= '9')) {
            number = number * 10 + car - '0';
          }
        }
      }
    }
  }

  return number;
}

void read_sensor_distance(){
  // Code from: https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
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
