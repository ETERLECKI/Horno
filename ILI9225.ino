#include <MAX6675.h>

/* ILI9225 Driver */
/* MEGA */
/* https://github.com/Nkawu/TFT_22_ILI9225 */


// Include application, user and local libraries
#include "SPI.h"
#include "TFT_22_ILI9225.h"

#define TFT_RST 46
#define TFT_RS  49
#define TFT_CS  53  // SS
#define TFT_SDI 51  // MOSI
#define THERMO_OUT 50 // MISO
#define THERMO_CS 45
#define TFT_CLK 52  // SCK
#define TFT_LED 48   // 0 if wired to +5V directly

/*
Hardware hookup

LCD Pin                 Arduino Pin          Mega
-------                 -----------          ----
 1  Vcc                 Vcc
 2  Gnd                 Gnd
 3  Gnd                 Gnd or N/C
 4                      N/C
 5                      N/C
 6  LED (backlight)     D3
 7  CLK                 D13 (SCK)             52
 8  SDI                 D11 (MOSI)            51
 9  RS                  D9
10  RST                 D8
11  CS                  D10 (SS)              53
*/

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED);
// Use software SPI (slower)
//TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);

MAX6675 termo(THERMO_CS);

// Variables and constants display
uint16_t x, y;
boolean flag = false;


// Variables and constants encoder
const int channelPinA = 2;
const int channelPinB = 3;
 
const int timeThreshold = 5;
long timeCounter = 0;
 
const int maxSteps = 250;
volatile int ISRCounter = 100;
int counter = 100;
float temp1;
String contador;
 
bool IsCW = true;

// Setup
void setup() {

  //Pines encoder
   pinMode(channelPinA, INPUT_PULLUP);
   pinMode(channelPinB, INPUT_PULLUP);
   //Interrupciones encoder
   attachInterrupt(digitalPinToInterrupt(channelPinA), doEncodeA, CHANGE);
   attachInterrupt(digitalPinToInterrupt(channelPinB), doEncodeB, CHANGE);
   
  tft.begin();
  Serial.begin(9600);

  tft.setOrientation(1);
  tft.setFont(Terminal12x16);
  tft.setBackgroundColor(COLOR_BLUE);
  tft.drawText(0, 0, "      TEMPERATURA      ",COLOR_WHITE);
  delay(500);
}

// Loop
void loop() {

     //if (counter != ISRCounter)
   //{
      counter = ISRCounter;
      contador=String(counter);
      //contador = contador + "C";
      temp1 = termo.readTempC();
      Serial.println(counter);
      Serial.println(temp1);
      tft.setFont(Trebuchet_MS16x21);
      tft.setBackgroundColor(COLOR_BLACK);
      tft.drawText(60, 100,contador ,COLOR_WHITE);
      tft.drawText(30,50,String(int(temp1)),COLOR_WHITE);
   //}
   delay(500);
   
 
 /* 
  tft.setBacklight(false);
  tft.setDisplay(false);
 */

}

void doEncodeA()
{
   if (millis() > timeCounter + timeThreshold)
   {
      if (digitalRead(channelPinA) == digitalRead(channelPinB))
      {
         IsCW = true;
         if (ISRCounter + 1 <= maxSteps) ISRCounter++;
      }
      else
      {
         IsCW = false;
         if (ISRCounter - 1 > 100) ISRCounter--;
      }
      timeCounter = millis();
   }
}
 
void doEncodeB()
{
   if (millis() > timeCounter + timeThreshold)
   {
      if (digitalRead(channelPinA) != digitalRead(channelPinB))
      {
         IsCW = true;
         if (ISRCounter + 1 <= maxSteps) ISRCounter++;
      }
      else
      {
         IsCW = false;
         if (ISRCounter - 1 > 0) ISRCounter--;
      }
      timeCounter = millis();
   }
}
