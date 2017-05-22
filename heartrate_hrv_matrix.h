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

//Variables for HRV Calcul
float moyRR = 0 ;
float varRR = 0 ;
int N = 50 ;
int buffer[50] = {0} ;

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
  matrix.begin();
  
  //Color background matrix
  matrix.fillScreen(matrix.Color888(30, 0, 255));
  
  //Draw Heart
  drawHeart();

  //Montre rouge
 // Serial.print("AT+CO1F9EB97EE2F88");
  //Montre bleu
  Serial.print("AT+CO1EF77CACD625F");
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
 * Used for set the delay for heart animation
 * There is 10 delay
 * 100 corresponds to 60 bpm
 */
int timing = 100; 

//Test for color background
int incre = 0;
void loop() {

//BACKGROUND COLOR
//Loop for color background (around the heart)
 for (int row = 0 ; row < 7; row ++)
  {
    for (int col = 0; col < 16 ; col++)
    {
      if (AroundHeart[row][col] == 3 )
      {
        matrix.drawPixel(originX + row,col, matrix.Color888(255 - incre, 0,20 + incre ));
      }
      /*if (HeartEmpty[row][col] == 2)
      {
        matrix.drawPixel(originX + row, originY + col, matrix.Color888(0, 0, 0));
     }*/
    }
  }
 matrix.fillRect(0,0,13,16,matrix.Color888(255 - incre, 0,20 + incre ));
 matrix.fillRect(20,0,32,16,matrix.Color888(255 - incre, 0,20 + incre ));
  incre = incre + 4;

  
 
  animationHeart(); //Call function for animate the heart

  //GET DATA FROM BLE MODULE
  if (Serial.available()) {
    i = 0;
    while (Serial.available())
    {
      //GET DATA FROM BLUETOOTH MODULE
      data = Serial.read();

      /*
       * ADD Data to table
       * Only 4 case because we only need one rrInterval
      */
      switch (i) {
        case 0:
          tableData[0] = data;
          break;
        case 1:
          tableData[1] = data;
          timing = 6000/data; //Define the timing here 
          break;
        case 2:
          tableData[2] = data;
          break;
        case 3:
          tableData[3] = data;
          break;
      }
      i++;
      delay(3); //ADD delay for wait the next value and stay in the while loop
    }
    
    //SHOW TABLE (useless)
    Serial.print("[");
    for (i = 0; i < 6; i = i + 1) {
      Serial.print(tableData[i]);
      Serial.print(",");
    }
    Serial.println("]");
    /*
     * rrInterval is on 16 bit (2 byte)
     * So take the two values and swap the value 2 before value 1
     * See Bluetooth Heart rate documentation
     */
    rrValue = ((uint16_t)tableData[3] << 8) | tableData[2];
    //SHOW RRValue (useless)
    Serial.println("RRValue: ");
    Serial.println(rrValue);

  }

  float HRV = computeHRV(rrValue) ;
  
  Serial.print(HRV);
}


void drawHeart()
{
  /*
   * Draw the heart by following the array
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
   * Draw the heart animation
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
   * HRV Calculation
   */
  RR = RR/1000 ;
  float dMoy = (RR - buffer[0])/N ;
  moyRR = moyRR + dMoy ;

  float dVar = dMoy * (RR + buffer[0] - 2*moyRR - dMoy) ;
  varRR = varRR + dVar ;

  for (int i = 0 ; i < N - 1 ; i++)
    buffer[i] = buffer[i+1] ;
  buffer[N-1] = RR ;

  return varRR ;
}
