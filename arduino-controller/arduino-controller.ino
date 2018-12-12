#include <Wire.h>
#include <math.h>

#define DEVICE (0x53)    //ADXL345 device address
#define TO_READ (6)        //num of bytes we are going to read each time (two bytes for each axis)

#define offsetX   -10.5       // place your OFFSET values here
#define offsetY   -2.5
#define offsetZ   -4.5

#define gainX     257.5        // place your GAIN factors
#define gainY     254.5
#define gainZ     248.5

/*
* Group Number 4:
* Group Members: Pau Balaguer, Didac Florensa and Hongzhi Zhu
*/


byte buff[TO_READ] ; //6 bytes buffer for saving data read from the device
char str[512]; //string buffer to transform data before sending it to the serial port

// X Y Z default values
int x = 0;
int y = 0;
int z = 0;

float base_pitch = 0;
float base_roll = -5;

//HeartRate sensor
int pulsePin = A0; 
int blinkPin = 0;
volatile int BPM;
volatile boolean QS;
volatile int Signal;
volatile int IBI;
volatile boolean Pulse;
int lastBPM;

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  
  //Turning on the ADXL345
  writeTo(DEVICE, 0x2D, 0);      
  writeTo(DEVICE, 0x2D, 16);
  writeTo(DEVICE, 0x2D, 8);
  interruptSetup();
}

void loop()
{
  int regAddress = 0x32; //first axis-acceleration-data register on the ADXL345
  
  readFrom(DEVICE, regAddress, TO_READ, buff); //read the acceleration data from the ADXL345

  get_coords(); // get the x, y and z coords
  String movement = get_movement(); // Get the movement string
  if(QS) lastBPM = BPM;
  Serial.println(movement + "-" + lastBPM);
  
  delay(20);
}

// Writes val to address register on device
void writeTo(int device, byte address, byte val) {
   Wire.beginTransmission(device);
   Wire.write(address);
   Wire.write(val);
   Wire.endTransmission();
}

// Reads num bytes starting from address register on device in to buff array
void readFrom(int device, byte address, int num, byte buff[]) {
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.beginTransmission(device);
  Wire.requestFrom(device, num);
  
  int i = 0;
  while(Wire.available()){ 
    buff[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
}

// Get X Y Z values
void get_coords(){
  x = (((int)buff[1]) << 8) | buff[0];   
  y = (((int)buff[3]) << 8) | buff[2];
  z = (((int)buff[5]) << 8) | buff[4];
}

String get_movement(){

  //Code from: https://www.dfrobot.com/wiki/index.php/How_to_Use_a_Three-Axis_Accelerometer_for_Tilt_Sensing#Yaw-Pitch-Roll
  double roll = 0.00, pitch = -5.00;   //Roll & Pitch are the angles which rotate by the axis X and y 

  double x_Buff = float(x);
  double y_Buff = float(y);
  double z_Buff = float(z);
  roll = atan2(y_Buff , z_Buff) * 57.3;
  pitch = atan2((- x_Buff) , sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3;

  if(abs(roll - base_roll) > abs(pitch - base_pitch)){
     if(roll < base_roll - 5) return "DOWN";
     if(roll > base_roll + 5) return "UP";
  }
  else{
    if(pitch < base_pitch - 5) return "RIGHT";
    if(pitch > base_pitch + 5) return "LEFT";
  }
  
  return "";
}
