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

/*
   Used for set the delay for heart animation
   There is 10 delay
   100 corresponds to 60 bpm
*/
int timing = 100;

void setup() {
  // init
  Serial.begin(9600);  // USB (choose 115200 in terminal)
  Serial1.begin(9600);
  matrix.begin();
  Serial.print("Begin...");  
  //Color background matrix
  matrix.fillScreen(matrix.Color888(30, 0, 255));
  
  //Draw Heart
  drawHeart();
}
String data;
String bpm;
String rrMs;
void loop() {
  animationHeart(); //Call function for animate the heart
  if (Serial1.available())
  {
    //Get data with ":" afetr value
    bpm = Serial1.readStringUntil(':');
    rrMs = Serial1.readStringUntil(':');
    //AFFICHAGE
    Serial.print("BPM : ");
    Serial.println(bpm);
    Serial.print("RR : ");
    Serial.println(rrMs);
    //BPM
    timing = 6000 / bpm.toInt(); 
  }
  // Use for send data from arduino to central
  //Don't work
  /*if (Serial.available())
    Serial1.write(Serial.read());*/
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
