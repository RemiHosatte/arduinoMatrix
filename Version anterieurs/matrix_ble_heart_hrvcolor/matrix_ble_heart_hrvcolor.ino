#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
//INIT LED MATRIX
#define CLK 8  // MUST be on PORTB!
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

//INIT FOR BLE
SoftwareSerial mySerial(12, 13); // RX, TX  

//SHAPES HEART
int Heart[9][9] = 
{
{0, 1, 1, 1, 0, 1, 1, 1, 0},
{1, 1, 1, 1, 1, 1, 1, 1, 1},
{1, 1, 1, 1, 1, 1, 1, 1, 1},
{0, 1, 1, 1, 1, 1, 1, 1, 0},
{0, 0, 1, 1, 1, 1, 1, 0, 0},
{0, 0, 0, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 0, 1, 0, 0, 0, 0}
};

int HeartEmpty[9][9] =
{
{0, 1, 1, 1, 0, 1, 1, 1, 0},
{1, 0, 0, 1, 1, 1, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 1},
{0, 1, 0, 0, 0, 0, 0, 1, 0},
{0, 0, 1, 0, 0, 0, 1, 0, 0},
{0, 0, 0, 1, 0, 1, 0, 0, 0},
{0, 0, 0, 0, 1, 0, 0, 0, 0}
};
//For align heart to center
int originX = 13;
int originY = 4;

//timing for animation
int timing = 100;


//Variables for ble 
byte tableData[6];
byte response;
int i;
uint16_t rrValue;

void setup() {
 Serial.begin(9600);
 mySerial.begin(9600);
 matrix.begin();

//AT COMMANDS FOR CONNECT TO HEART RATE SENSOR
  mySerial.print("AT+CO1F9EB97EE2F88");
  Serial.println("AT+CO1F9EB97EE2F88");
 
 //Create heart
 
for (int row = 0 ; row < 9 ; row ++)
  {

    
    for (int col = 0; col < 9 ; col++) 
    {
      
    if (HeartEmpty[row][col] == 1 )
    {
       matrix.drawPixel(originX + row,originY + col, matrix.Color888(50, 50, 50));
    }
   
    Serial.print(HeartEmpty[row][col]);
    Serial.print("\t");
    }
    Serial.println();
    
  }
}

void loop() {
  if (Serial.available()) {
    Serial.println();
    while(Serial.available())
    {
    response = Serial.read();
    mySerial.print(response);
   // Serial.print(response);
    }
    Serial.println();
   
  }
if (mySerial.available()) {
    i =0;
    while(mySerial.available())
    {
    
    response = mySerial.read();
    //Serial.print(response);
    
    
    switch (i) {
    case 0:
      tableData[0] = response;
    /*  for (int j = 7;j>=0;j--)
      {
         Serial.print(bitRead(5,j));
      }
     */
      break;
    case 1:
      tableData[1] = response;
      
      /*
  //For mini screen
  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(40,0);
  display.print(tableData[1]);
  display.display();
  */
  //For matrix LED
 
      break;
    case 2:
      tableData[2] = response;
      break;
    case 3:
      tableData[3] = response;
      break;
    }
    i++;
    //On patiente 1ms pour recevoir la donnée entiere
    delay(1);
    }
    
   /* Serial.print("[");
    for (i = 0; i < 6; i = i + 1) {
    Serial.print(tableData[i]);
    Serial.print(",");
    }
    Serial.print("]");*/


    rrValue = ((uint16_t)tableData[3] << 8) | tableData[2];
    Serial.print(rrValue);
    Serial.println();
 }
  /*
  //Animation for heart

//Ligne5
matrix.drawPixel(originX + 5,originY + 4, matrix.Color333(7, 0, 0));
delay(timing);

//Ligne4
matrix.drawLine(originX + 4,originY + 3,originX + 4 , originY + 5, matrix.Color333(7, 0, 0)); 
delay(timing);

//Ligne3
matrix.drawLine(originX + 3,originY + 2,originX + 3 , originY + 6, matrix.Color333(7, 0, 0));
delay(timing);

//Ligne2
matrix.drawLine(originX + 2,originY + 1,originX + 2 , originY + 7, matrix.Color333(7, 0, 0));
delay(timing);

//Ligne1
matrix.drawLine(originX + 1,originY + 6,originX + 1 , originY + 7, matrix.Color333(7, 0, 0));
matrix.drawLine(originX + 1,originY + 1,originX + 1 , originY + 2, matrix.Color333(7, 0, 0));
delay(timing);

//Ligne1
matrix.drawLine(originX + 1,originY + 6,originX + 1 , originY + 7, matrix.Color333(0, 0, 0));
matrix.drawLine(originX + 1,originY + 1,originX + 1 , originY + 2, matrix.Color333(0  , 0, 0));
delay(timing);
//Ligne2
matrix.drawLine(originX + 2,originY + 1,originX + 2 , originY + 7, matrix.Color333(0, 0, 0));
delay(timing);
//Ligne3
matrix.drawLine(originX + 3,originY + 2,originX + 3 , originY + 6, matrix.Color333(0, 0, 0));
delay(timing);
//Ligne4
matrix.drawLine(originX + 4,originY + 3,originX + 4 , originY + 5, matrix.Color333(0, 0, 0)); 
delay(timing);
//Ligne5
matrix.drawPixel(originX + 5,originY + 4, matrix.Color333(0, 0, 0));
delay(timing);
*/


}
