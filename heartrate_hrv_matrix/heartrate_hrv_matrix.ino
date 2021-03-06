#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
//INIT LED MATRIX
#define CLK 11  // MUST be on PORTB!
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

//Variables for HRV Calcul
float moyRR = 0 ;
float varRR = 0 ;
int N = 50 ;
int buffer[50] = {0} ;

int iconConnect[3][5] {
  {1,0,0,0,1},
  {0,1,3,1,0},
  {1,0,0,0,1}
};

//Matrix for draw heart
int HeartEmpty[7][9] =
{
  {0, 1, 1, 1, 0, 1, 1, 1, 0},
  {1, 2, 2, 1, 1, 1, 2, 2, 1},
  {1, 2, 2, 2, 2, 2, 2, 2, 1},
  {0, 1, 2, 2, 2, 2, 2, 1, 0},
  {0, 0, 1, 2, 2, 2, 1, 0, 0},
  {0, 0, 0, 1, 2, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0}
};

//Matrix for background color around the heart
int AroundHeart[7][16] =
{
  {3, 3, 3, 3, 3, 0, 0, 0, 3, 0, 0, 0, 3, 3, 3, 3},
  {3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3},
  {3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3},
  {3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3},
  {3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3},
  {3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 3, 3},
  {3, 3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3}


};

//For align heart to center
int originX = 13;
int originY = 4;

void setup() {
  //Open serial
  Serial.begin(9600);
  Serial1.print(9600);
  matrix.begin();

  //Color background matrix
  matrix.fillScreen(matrix.Color888(30, 0, 255));

//Draw Heart
  drawHeart();

  
  //Montre rouge
  // Serial.print("AT+CO1F9EB97EE2F88");
  //Montre bleu
  //Serial.print("AT+CO1EF77CACD625F");
  //delay(2500);
  Serial.print("Begin..");
}


uint8_t data; //Data from ble module
int i; //Use for create table with data from ble module

/*
  Table with data from ble module
  [Flags,HeartRate,rrInterval,rrInterval,...]
*/
byte tableData[6];

uint16_t rrValue; //RRvalue in millisecond

/*
   Used for set the delay for heart animation
   There is 10 delay
   100 corresponds to 60 bpm
*/
int timing = 100;

//Test for color background
int incre = 20;
void loop() {

  //GET DATA FROM BLE MODULE
  if (Serial1.available())
  {
    Serial.print("ok");
    Serial.write(Serial1.read()); 
  }
  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
  
    Serial1.write(Serial.read());
 
}


void drawHeart()
{
  /*
     Draw the heart by following the array
  */
  for (int row = 0 ; row < 9 ; row ++)
  {
    for (int col = 0; col < 9 ; col++)
    {
      if (HeartEmpty[row][col] == 1 )
      {
        matrix.drawPixel(originX + row, originY + col, matrix.Color888(255, 0, 0));
      }
      if (HeartEmpty[row][col] == 2)
      {
        matrix.drawPixel(originX + row, originY + col, matrix.Color888(0, 0, 0));
      }
    }
  }
}


void animationHeart()
{
  /*
     Draw the heart animation
  */
  //Draw the heart up
  for (int nb = 0; nb < 4; nb++)
  {
    matrix.drawLine(originX + 5 - nb , originY + 4 - nb, originX + 5 - nb, originY + 4 + nb, matrix.Color888(255, 0, 50));
    delay(timing);
  }

  //Hightest line
  matrix.drawLine(originX + 1, originY + 6, originX + 1 , originY + 7, matrix.Color888(255, 0, 19));
  matrix.drawLine(originX + 1, originY + 1, originX + 1 , originY + 2, matrix.Color888(255, 0, 19));
  delay(timing);

  //Draw the heart down
  matrix.drawLine(originX + 1, originY + 6, originX + 1 , originY + 7, matrix.Color333(0, 0, 0));
  matrix.drawLine(originX + 1, originY + 1, originX + 1 , originY + 2, matrix.Color333(0  , 0, 0));
  delay(timing);

  for (int nb = 0; nb < 4; nb++)
  {
    matrix.drawLine(originX + 2 + nb, originY + 1 + nb , originX + 2 + nb, originY + 7 - nb, matrix.Color333(0, 0, 0));
    delay(timing);
  }

}



float computeHRV(float RR)
{
  /*
     HRV Calculation
  */
  RR = RR / 1000 ;
  float dMoy = (RR - buffer[0]) / N ;
  moyRR = moyRR + dMoy ;

  float dVar = dMoy * (RR + buffer[0] - 2 * moyRR - dMoy) ;
  varRR = varRR + dVar ;

  for (int i = 0 ; i < N - 1 ; i++)
    buffer[i] = buffer[i + 1] ;
  buffer[N - 1] = RR ;

  return varRR ;
}
