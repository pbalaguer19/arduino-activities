#include  <Wire.h>                // I2C access

/*
* Group Number 4:
* Group Members: Pau Balaguer, Didac Florensa and Hongzhi Zhu
* Exercise 2.2 [Emissor Arduino]
*/


// ULTRA SOUND SENSOR Settings
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo
long duration, cm;


//MIN and MAX variables
int min_cm, max_cm;

void setup(){
  Serial.begin(9600);

  //USS pinMode
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //LED pinMode
  pinMode(LED_BUILTIN, OUTPUT);

  read_min_max_values();
}

void loop(){
  //Read the sensor distance and sent it to Receiver Arduino
  read_sensor_distance();
  send_distance();
  
  if(!(cm >= min_cm && cm <= max_cm)) open_led();
    
  delay(1000);
}

void read_min_max_values(){
  min_cm = read_serial_integer();
  max_cm = read_serial_integer();

  if (max_cm <= min_cm) max_cm = min_cm + 2;
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

void send_distance(){
  Serial.print(cm);
}

void open_led(){  
  // Turn on led
  digitalWrite(LED_BUILTIN, HIGH);
}
