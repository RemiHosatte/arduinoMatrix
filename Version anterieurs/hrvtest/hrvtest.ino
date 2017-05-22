// testcolors demo for Adafruit RGBmatrixPanel library.
// Renders 512 colors on our 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420
// Library supports 4096 colors, but there aren't that many pixels!  :)

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <SoftwareSerial.h>

#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

  
  int red = 0;
  int blue = 0;
  int x=32;
  int y= 0;
void setup() {
  Serial.begin(9600); 
  matrix.begin();
   matrix.fillRect(0, 0, 32, 16, matrix.Color888(10, 0, 255));
  
} 
void blueToRed()
{
  int randNum =random(25);
  Serial.print(randNum);
  Serial.print("-");
  for (x=32; x >= 0; x--) {

    
    matrix.drawLine(x-1, 0, x-1, 15, matrix.Color888(100, 0, 160));
    matrix.drawLine(x-2, 0, x-2, 15, matrix.Color888(70, 0, 190));
    matrix.drawLine(x-3, 0, x-3, 15, matrix.Color888(40, 0, 220));
    matrix.drawLine(x-4, 0, x-4, 15, matrix.Color888(10, 0, 250));
    
    matrix.drawLine(x, 0, x, 15, matrix.Color888(130, 0, 130));
   
    matrix.drawLine(x+1, 0, x+1, 15, matrix.Color888(160, 0, 100));
    matrix.drawLine(x+2, 0, x+2, 15, matrix.Color888(190, 0, 70));
    matrix.drawLine(x+3, 0, x+3, 15, matrix.Color888(220, 0, 40));
    matrix.drawLine(x+4, 0, x+4, 15, matrix.Color888(250, 0, 25));
    
    delay(10 + randNum);
  }
  for (x=0; x <= 32; x++) {
    
    
    matrix.drawLine(x-1, 0, x-1, 15, matrix.Color888(100, 0, 160));
    matrix.drawLine(x-2, 0, x-2, 15, matrix.Color888(70, 0, 190));
    matrix.drawLine(x-3, 0, x-3, 15, matrix.Color888(40, 0, 220));
    matrix.drawLine(x-4, 0, x-4, 15, matrix.Color888(20, 0, 250));

    matrix.drawLine(x, 0, x, 15, matrix.Color888(130, 0, 130));

     matrix.drawLine(x+1, 0, x+1, 15, matrix.Color888(160, 0, 100));
    matrix.drawLine(x+2, 0, x+2, 15, matrix.Color888(190, 0, 70));
    matrix.drawLine(x+3, 0, x+3, 15, matrix.Color888(220, 0, 40));
    matrix.drawLine(x+4, 0, x+4, 15, matrix.Color888(250, 0, 25));
    
    delay(10 + randNum);
  }
 
}


void loop() {
 blueToRed();
 // matrix.drawLine(1, 0, 1, 15, matrix.Color888(10, 0, 255));

 //matrix.drawLine(31, 0, 31, 15, matrix.Color888(255, 0, 10));
 
 
}

















/*
void effects(int position)
{
  for (int x=0; x < 8; x++) {
    
    
    red = red + 30;
    
    blue = blue - 30;
        
      matrix.drawLine(x, 0, x, 15, matrix.Color888(red, 0, blue));
     
      delay(10);
  }
}

void loop() {
  int randomNum = random(32);
  Serial.print(randomNum);
  Serial.print("-");
  
//effects(randomNum);
delay(500);
}
*/
