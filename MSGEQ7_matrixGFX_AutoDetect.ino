/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  MSGEQ7 Serial example
  Output via Serial

  Needs no special hardware and displays raw values on the Serial monitor.

  Reads MSGEQ7 IC with 7 different frequencys from range 0-255
  63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
*/
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include<EEPROM.h>
// MSGEQ7
#include "MSGEQ7.h"
#define pinAnalog A0
#define pinReset 5
#define pinStrobe 7

#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH false


#define mw 8 //lebar led matrix
#define mh 20 //tinggi led matrix
//sett bistro
//#define mw 12 //lebar led matrix
//#define mh 9 //tinggi led matrix

//#define PIN 9 //for test
#define PIN 10

//mulai kabel pertama dari atas ke bawah
//Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(mw, mh, PIN,
//                            NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
//                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
//                            NEO_BRG            + NEO_KHZ800);
                      
//mulai kabel pertama dari bawah ke atas
//Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(mw, mh, PIN,
//                            NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
//                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
//                            NEO_BRG            + NEO_KHZ800);
                                  
//mulai kabel pertama dari atas kiri ke kanan
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(mw, mh, PIN,
                            NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                            NEO_BRG            + NEO_KHZ800);

//mulai kabel pertama dari atas kiri ke kanan
//Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(mw, mh, PIN,
//                            NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
//                            NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
//                            NEO_GRB            + NEO_KHZ800);


#define BLACK    0

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalog> MSGEQ7;


int speedFade = 1500 / mh;
int speedFadeMax = 3000 / mh;
byte jumFrek = 9;


int frekMin[] = {500, 500, 500, 500, 500, 500, 500, 500, 500};
int freqMax[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
int lastFrekMin[] = {500, 500, 500, 500, 500, 500, 500, 500, 500};
int lastFrekMax[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int deltaFrek[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

float v[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float valMin[] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
float value[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float valueMax[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int fade = 0;
float valMax = 1000;
int ledMin = 0;
//float valMin = 10;
bool addOnSwitch = 0;
int brightVal;
int countIn = 0;
unsigned long Tmr, lastTmr;
unsigned long TmrMax, lastTmrMax;
byte modee = 0;
int clr = 0;
int clrAddOn = 0;
int conn = 0;

void setup() {
  // This will set the IC ready for reading
  MSGEQ7.begin();

  // Serial setup with faster baud rate gives more FPS (115200)
  Serial.begin(115200);
  Serial.println(F("Startup"));
  matrix.begin();
  for (int con = 0; con < 9; con++) {
    valMin[con] = EEPROM.read(con);
    Serial.print(valMin[con]);
    Serial.print(F(" \t"));
  }
  Serial.println(" EEPROM read success!!");
  matrix.fillScreen(matrix.Color(255, 0, 0));
  matrix.show();
  delay(1000);
  matrix.fillScreen(matrix.Color(0, 255, 0));
  matrix.show();
  delay(1000);
  matrix.fillScreen(matrix.Color( 0, 0, 255));
  matrix.show();
  delay(1000);
}

void loop() {
  // Analyze without delay every interval
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Calculate FPS
  uint16_t FPS = getFPS(newReading);

  // Serial raw debug output
  if (newReading)
    serialBars(FPS);
}

uint16_t getFPS(bool newReading) {
  // Variables to count FPS and last 1 second mark
  static uint16_t prevFPS = 0;
  static uint16_t FPS = 0;
  static uint32_t m = 0;

  // Increase FPS count
  if (newReading)
    FPS++;

  // If 1 second mark crossed, save new FPS
  if ((micros() - m) > 1000000) {
    prevFPS = FPS;
    FPS = 0;
    m = micros();
  }

  return prevFPS;
}

uint32_t WheelMatrix(int WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos == -1 ) {
    return matrix.Color(0, 0, 0);
  } else if (WheelPos == -2) {
    return matrix.Color(255, 255, 255);
  } else if (WheelPos < 85) {
    return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

uint32_t FireMatrix(int WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return matrix.Color(255, 255, 255 - (WheelPos * 3));
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return matrix.Color(255, 255 - (WheelPos * 3), 0);
  } else {
    return matrix.Color(255, 0, 0);
  }
}

uint32_t RedBlue(int WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 128) {
    return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 128;
    return matrix.Color(WheelPos * 3, 0, 255 - WheelPos * 3);
  }
}

