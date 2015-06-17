// include the lirary code
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
/**********************************************************/
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);
SoftwareSerial mySerial(0, 1); // RX, TX
/*********************************************************/
void setup()
{
  lcd.begin(16, 2);  // set up the LCD's number of columns and rows: 
  Serial.begin(9600);
  mySerial.begin(9600);
}
/*********************************************************/
void loop() 
{
   if(mySerial.available()){
      char c = mySerial.read();
      if(c == 'q'){
        lcd.clear();
      }
      else if(c == '!'){
        lcd.setCursor(0,0);   
      }
      else if(c == '@'){
        lcd.setCursor(0,1);
      }
      else lcd.print(c);
   }
}
/************************************************************/
