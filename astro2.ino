
/*
   ASTROINSEGUITORE versione 02
   (c) 2016 Lafayette Software Development for Il Poliedrico
   written by Umberto Genovese

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
  //
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  //
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
  //



  Hardware richiesto:
  1 Motherboard Arduino Mega 2560 R3 ATmega2560-16AU
  1 GY-GPS6MV1 ricevitore GPS per l'altezza del Polo Celeste (asse polare)
  1 MPU6050 accellerometro-giroscopio 6 gradi di libertà
  1 MPL3115A2 barometro - altimetro - termometro
  1 HMC5983 bussola eletttronica
  1 Controller RepRap con monitor 12864 (seriale), buzzer, encoder, letttore SD e pulsante integrati

*/

#include "U8glib.h"
#include "Button.h"
#include "RotaryEncoder.h"
#include "pitches.h"
#include "Wire.h"
#include "SparkFunMPL3115A2.h"
//#include "time.h"
#include "NMEAGPS.h"
#include "DMS.h"
#include "AccelStepper.h"
//#include <MultiStepper.h>



RotaryEncoder encoder(40, 42);
Button ENTER (46,  BUTTON_PULLUP_INTERNAL);
Button ESCAPE( 39,  BUTTON_PULLUP_INTERNAL);
#define gpsSerial Serial1
#define SCATTO   11      // CNY 75B
#define SINISTRA 10
#define BOLLA     9
#define DESTRA    8

#define EN-       7      // TBH 7128
#define EN+       6      // TBH 7128
#define PUL-      5      // TBH 7128
#define PUL+      4      // TBH 7128
#define DIR-      3      // TBH 7128
#define DIR+      2      // TBH 7128


 
#define SD_DET     38   // EXP2 7
// PIN ESCAPE           // EXP2 8
// PIN 40 ENCODER       // EXP2 5
#define SD_MOSI    41   // EXP2 6
// PIN 42 ENCODER       // EXP2 3
// PIN 43 NON USATO     // EXP2 4
#define SD_MISO    44   // EXP2 1
#define SD_SCK     45   // EXP2 2

// PIN 46 P. SELETTORE  // EXP1 2 
#define BUZZER_PIN 47   // EXP1 1 
#define CS_RS      48   // EXP1 4
#define RW_SID     49   // EXP1 3
// PIN 50 NON USATO     // EXP1 6
#define E_SCLK     51   // EXP  5
// PIN 51 NON USATO     // EXP1 8
// PIN 52 NON USATO     // EXP1 7
// Vin +5 SCUDO LCD     // EXP1 10
//  GND   SCUDO LCD     // EXP1 9


#define MPU 0x68  // I2C address of the MPU-6050
U8GLIB_ST7920_128X64_1X u8g( E_SCLK, RW_SID, CS_RS );
MPL3115A2 sensor;
NMEAGPS  GPS;
gps_fix  fix;


const int melody[] = {
  NOTE_D7, NOTE_E7, NOTE_C7, NOTE_C6, NOTE_G6 //Musichina
};
const int noteDurations[] = {  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  9, 7, 5, 3, 1
};

#define MENU_ITEMS 4
char *menu_strings[6] =
{ "Impostazioni", "Calibrazione", "Start", "Reset", "0", "3"};
String _buffer = "  ";

uint8_t menu_current = 0;
static int REFRESH  ;
static int newPos = 0 ;
static int pos = 0 ;
double  AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
//long gyro_x_cal, gyro_y_cal, gyro_z_cal; 

float Inclinazione, Altezza;
const float pi PROGMEM = 3.1415926535898;   // Angolo di inizio
const float GIORNO_SOLARE PROGMEM = 86400; // Giorno solare in secondi
const float GIORNO_SIDERALE PROGMEM = 86164.0419; // Giorno siderale in secondi e decimali
const float SEC_SIDERALE = 1 / ( GIORNO_SIDERALE / GIORNO_SOLARE );  // Converte un secondo in tempo siderale

float pressione = 0.0;
float tempC = 0.0;
float quota = 0.0;


void setup()
{
  if (u8g.getMode() == U8G_MODE_R3G3B2)        {
    u8g.setColorIndex(255);
  }
  else if (u8g.getMode() == U8G_MODE_GRAY2BIT) {
    u8g.setColorIndex(3);
  }
  else if (u8g.getMode() == U8G_MODE_BW)       {
    u8g.setColorIndex(1);
  }
  else if (u8g.getMode() == U8G_MODE_HICOLOR)  {
    u8g.setHiColorByRGB(255, 255, 255);
  }
  pinMode (SINISTRA, OUTPUT);
  pinMode (BOLLA, OUTPUT);
  pinMode (DESTRA, OUTPUT);

  Serial.begin(115200);
  sensor.begin();
  sensor.setModeAltimeter(); // Measure quota above sea level in meters
  sensor.setModeBarometer(); // Measure pressione in Pascals from 20 to 110 kPa
  sensor.setOversampleRate(7); // Set Oversample to the recommended 128
  sensor.enableEventFlags(); // Enable all three pressione and temp event flags


  gpsSerial.begin(9600);
  gpsSerial.println( F("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28") ); // RMC & GGA only
  gpsSerial.println( F("$PMTK220,1000*1F") ); // 1Hz update rate
  waitForFix();

  init_MPU(); // Inizializzazione MPU6050

  Init_Splash_Draw ();
} // setup()

void UpdateMenu () {

  int ROTARY_MIN = atoi ( menu_strings[4] ) ;
  int ROTARY_MAX = atoi ( menu_strings[5] ) ;
  encoder.tick();
  // just call tick() to check the state.
  newPos = encoder.getPosition();

  if (newPos < ROTARY_MIN ) {
    encoder.setPosition(ROTARY_MAX );
    newPos = ROTARY_MAX;

  } else if (newPos > ROTARY_MAX ) {
    encoder.setPosition(ROTARY_MIN );
    newPos = ROTARY_MIN;
  } else {
    newPos = encoder.getPosition() ;
  }
  if (pos != newPos) {
    pos = newPos;
    Serial.print(newPos);
    Serial.println();

    tone (BUZZER_PIN, 440 + (440 * newPos ), 50);

  } // if
  menu_current  = newPos;
  draw_menu();
  if (ENTER.isPressed() ) {
    tone(BUZZER_PIN, NOTE_E4, 50);
    switch (menu_current) {
      case 0:
        Musichina();
        break;
      case 1:
        while (ESCAPE.isPressed () == false) {
          SENSOR();
        }
        break;
      case 2:
        while (ESCAPE.isPressed () == false) {
          LIVELLA();
        }
        break;
      case 3:
        while (ESCAPE.isPressed () == false) {
          printFix();
        }
        break;
      default:
        delay(1);
    }
  }

}

void draw_menu() {
  uint8_t i, h, h1;
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();

  h = u8g.getFontAscent() - u8g.getFontDescent();
  w = u8g.getWidth();
  for ( i = 0; i < atoi ( menu_strings[5]) + 1 ; i++ ) {
    d = (w - u8g.getStrWidth(menu_strings[i])) / 2;
    h1 = (h + 3) * i;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(0, h1 + 1, w, h);
      u8g.setDefaultBackgroundColor();


    }
    u8g.drawStr(d, h1, menu_strings[i]);

  }

}

// Read the current position of the encoder and print out when changed.
void loop()
{

  u8g.firstPage();
  do {
   //
     UpdateMenu ();

  } while ( u8g.nextPage() );

} // loop ()




// The End


