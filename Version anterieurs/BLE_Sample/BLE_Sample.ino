#include <BLESerial.h>

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

SoftwareSerial mySerial(12, 13); // RX, TX
// Connect HM10      Arduino Uno
//     Pin 1/TXD          Pin 7
//     Pin 2/RXD          Pin 8

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.drawPixel(0,0, matrix.Color333(7,7,7));
  // If the baudrate of the HM-10 module has been updated,
  // you may need to change 9600 by another value
  // Once you have found the correct baudrate,
  // you can update it using AT+BAUDx command
  // e.g. AT+BAUD0 for 9600 bauds
  mySerial.begin(9600);
  //Montre rouge
 // mySerial.print("AT+CO1F9EB97EE2F88");
 // Serial.println("AT+CO1F9EB97EE2F88");
  //Montre bleu
   mySerial.print("AT+CO1EF77CACD625F");
  Serial.println("AT+CO1F9EB97EE2F88");



}
byte tableData[6];
byte response;
int i;
uint16_t rrValue;
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
      for (int j = 7;j>=0;j--)
      {
         Serial.print(bitRead(5,j));
      }

      break;
    case 1:
      tableData[1] = response;
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
    Serial.print("[");
    for (i = 0; i < 6; i = i + 1) {
    Serial.print(tableData[i]);
    Serial.print(",");
    }
    Serial.print("]");


    rrValue = ((uint16_t)tableData[3] << 8) | tableData[2];
    Serial.print(rrValue);
    Serial.println();
 }



}
