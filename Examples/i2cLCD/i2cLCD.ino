


#include  <Wire.h>                // I2C access
#include  <LiquidCrystal_I2C.h>   // LCD library

// LCD 16x2 on I2C 0x27 address
// LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{

  Serial.begin(9600);
  
  // lcd.begin(16, 2); // Columns & Rows
  lcd.init();
  lcd.home ();      // Set (X,Y)=(0,0)
  
  lcd.print("Hello World");
  lcd.setCursor ( 0, 1 );
  lcd.print("  ARDUINO");
  
  delay(500);
}

void loop()
{
  int i;

  // Scrool right
  for ( int i = 0; i < 5; i++ ) {
    lcd.scrollDisplayRight();
    delay (500);
  }

  // Scroll left
  for ( int i = 0; i < 5; i++ ) {
    lcd.scrollDisplayLeft();
    delay (500);
  }
  
}
