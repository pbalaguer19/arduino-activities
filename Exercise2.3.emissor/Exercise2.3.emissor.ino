#include <Wire.h>

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
* Exercise 2.3.emissor
*/


byte buff[TO_READ] ; //6 bytes buffer for saving data read from the device
char str[512]; //string buffer to transform data before sending it to the serial port

// X Y Z default values
int x = 0, previous_x = 0;
int y = 0, previous_y = 0;
int z = 0, previous_z = 0;


void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  
  //Turning on the ADXL345
  writeTo(DEVICE, 0x2D, 0);      
  writeTo(DEVICE, 0x2D, 16);
  writeTo(DEVICE, 0x2D, 8);
}

void loop()
{
  int regAddress = 0x32; //first axis-acceleration-data register on the ADXL345
  
  readFrom(DEVICE, regAddress, TO_READ, buff); //read the acceleration data from the ADXL345

  get_coords(); // get the x, y and z coords
  String movement = get_movement(); // Get the movement string
  Serial.print(movement);
  
  delay(2000);
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
  previous_x = x;
  previous_y = y;
  previous_z = z;
  
  x = (((int)buff[1]) << 8) | buff[0];   
  y = (((int)buff[3])<< 8) | buff[2];
  z = (((int)buff[5]) << 8) | buff[4];
}

String get_movement(){
  if(x > previous_x) return "RIGHT";
  if(x < previous_x) return "LEFT";
  if(z > previous_z) return "UP";
  if(z < previous_z) return "RIGHT";
  return "";
}
