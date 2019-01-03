#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>


#define ARDUINO_ADDRESS 0x04

int main (void){
  char tmp[1];
  char ch;
  char fd;
  char values[50];

  wiringPiSetup();
  fd = wiringPiI2CSetup(ARDUINO_ADDRESS);

  do{
    ch = (char) wiringPiI2CRead(fd);
    printf("%d\n", ch);
    //tmp[0] = ch;
    //strcat(values, tmp);
  } while (ch != '%');

  return (0);
}
