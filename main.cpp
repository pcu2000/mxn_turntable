//
// AUTHOR: Martin Ming MMA
// PURPOSE: test PCA9685 library
// URL: https://github.com/RobTillaart/PCA9685_RT
//
//  sets one channel to max PWM 0..4095
//  and connect the output to an interrupt pin 2
//  to see the frequency of the PWM


#include "PCA9685.h"


PCA9685 PCA[] = {
  PCA9685(0x40), //Pannel 1
  PCA9685(0x41),
  PCA9685(0x42),
  PCA9685(0x43), 
  PCA9685(0x44),
  PCA9685(0x45), 
  PCA9685(0x46),
  PCA9685(0x47), 
  PCA9685(0x49),
  PCA9685(0x48),
  PCA9685(0x50), //Pannel 2
  PCA9685(0x51),
  PCA9685(0x52),
  PCA9685(0x53), 
  PCA9685(0x54),
  PCA9685(0x55), 
  PCA9685(0x56),
  PCA9685(0x57), 
  PCA9685(0x59),
  PCA9685(0x58),
  PCA9685(0x60), //Pannel 3
  PCA9685(0x61),
  PCA9685(0x62),
  PCA9685(0x63), 
  PCA9685(0x64),
  PCA9685(0x65), 
  PCA9685(0x66),
  PCA9685(0x67), 
  PCA9685(0x69),
  PCA9685(0x68) 
};

//const uint8_t NUM_PCA = sizeof(pca) / sizeof(pca[0]);
//PCA9685 PCA(0x40);
//PCA9685 PCA1(0x41);

//const uint8_t IRQ_PIN = 2;
//volatile uint16_t count = 0;
//uint32_t lastTime = 0;

#define LEDR 2
#define LEDG 1
#define LEDB 3
#define LEDW 0
#define PIN_OE D0
#define PIN_SDA D4
#define PIN_SCL D5

#define LEDMODULES 30
#define LEDPCS 4
#define LEDMAXVALUE 4095

const uint16_t ColorNumberPcs = 12;
static u_int16_t ledCollorsArray[ColorNumberPcs][4] = {
    {4095,0,0,0},
    {0,4095,0,0},
    {0,0,4095,0},
    {0,0,0,4095},
    {1211,0,0,0},
    {1211,1211,0,0},
    {0,1211,1211,0},
    {0,0,1211,1211},
    {0,0,0,1211},
    {457,457,457,0},
    {457,457,457,457},
    {0,0,0,0}
  };

const uint16_t sinLUT[100] = {
       0,    1,    3,    6,   10,   15,   21,   28,   36,   45,
      55,   66,   78,   91,  105,  120,  135,  152,  170,  189,
     208,  229,  251,  273,  297,  321,  346,  372,  400,  428,
     457,  486,  517,  549,  581,  614,  648,  683,  719,  756,
     793,  832,  871,  911,  951,  993, 1035, 1078, 1121, 1166,
    1211, 1257, 1303, 1350, 1398, 1446, 1496, 1545, 1596, 1647,
    1698, 1751, 1803, 1857, 1911, 1965, 2020, 2075, 2131, 2188,
    2244, 2302, 2360, 2418, 2476, 2535, 2595, 2654, 2714, 2775,
    2836, 2897, 2958, 3020, 3082, 3144, 3206, 3269, 3331, 3394,
    3458, 3521, 3584, 3648, 3712, 3775, 3839, 3903, 3967, 4031
};

void enableLED(bool enable);
void clearAllLed();
void setRGBWValue(u_int8_t led, u_int16_t ledR, u_int16_t ledG, u_int16_t ledB, u_int16_t ledW);
void setRGBWValue(u_int16_t element, u_int8_t led, u_int16_t ledR, u_int16_t ledG, u_int16_t ledB, u_int16_t ledW);
void setRGBWValueBar(u_int8_t row, u_int16_t ledR, u_int16_t ledG, u_int16_t ledB, u_int16_t ledW);
void setRGBWSlowOnOff(u_int8_t led, bool ledR, bool ledG, bool ledB, bool ledW, bool on, u_int16_t speed);

void checkHardware();


void setup()
{
  /*Serial.begin(115200);
    Serial.println(PCA9685_LIB_VERSION);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("PCA9685_LIB_VERSION: ");
  Serial.println(PCA9685_LIB_VERSION);
  Serial.println();*/
  delay(1000);
  pinMode(PIN_OE, OUTPUT);
  digitalWrite(PIN_OE, false);
  Wire.begin(PIN_SDA, PIN_SCL, 1000000);
   for (int i = 0; i < LEDMODULES; i++)
    {
    PCA[i].begin();
    PCA[i].setMode2(0b00011011); //Konfiguration für Gridfinity (LED mit Vorwiderstand auf 5V) (Datasheet NXP S. 16 MODE2)
    PCA[i].setFrequency(200); //default 200Hz (Datasheet NXP S. 25)
    }
  clearAllLed(); //da allOFF noch nicht funktioniert
  checkHardware();
  //PCA.allOFF(); //Funktioniert mit dieser Library und HW noch nicht. Library müsste angepasst werden

  delay(1000);
    // Serial port for debugging purposes
  //Serial.begin(115200);
  //Serial.print("Setup:");
}


void loop()
{ 
  //LED's einzeln Wert einstellen
  setRGBWValue(0, 4095, 0, 0, 0);
  setRGBWValue(1, 0, 4095, 0, 0);
  setRGBWValue(2, 0, 0, 4095, 0);
  setRGBWValue(3, 0, 0, 0, 4095);
  setRGBWValue(4, 4095, 0, 0, 0);
  setRGBWValue(5, 0, 4095, 0, 0);
  setRGBWValue(6, 0, 0, 4095, 0);
  setRGBWValue(7, 0, 0, 0, 4095);
  delay(1000);
  //alle LEDS deaktivieren über OE Signal
  enableLED(false);
  delay(1000);
  
  //alle LEDS aktivieren über OE Signal
  enableLED(true);
  delay(1000);
  clearAllLed();
  delay(1000);

  /*for (int z = 0; z < 20; z++)
    {
    for (int i = 0; i < 16; i++)
    {
      PCA[0].write1(i, HIGH);
      delay(2000);
      PCA[0].write1(i, LOW);
      delay(2000);
    }
    for (int i = 0; i < 16; i++)
    {
      PCA[1].write1(i, HIGH);
      delay(2000);
      PCA[1].write1(i, LOW);
      delay(2000);
    }
  }*/

  /*for (int z = 0; z < 1000; z++)
    {
    for (int i = 0; i < 16; i++)
    {
      PCA[0].write1(i, HIGH);
      delay(1);
    }
    for (int i = 0; i < 16; i++)
    {
      PCA[1].write1(i, HIGH);
      delay(1);
    }
    delay(200);
    for (int i = 0; i < 16; i++)
    {
      PCA[0].write1(i, LOW);
      delay(1);
    }
    for (int i = 0; i < 16; i++)
    {
      PCA[1].write1(i, LOW);
      delay(1);
    }
    //delay(200);
  }*/

  //Lichtablauf einzel per Array
  for (int i = 0; i < ColorNumberPcs; i++)
  {
    for(int z = 0; z < LEDMODULES; z++){
      for (int y = 0; y < 4; y++)
      {
        setRGBWValue(z, y, ledCollorsArray[i][0], ledCollorsArray[i][1], ledCollorsArray[i][2], ledCollorsArray[i][3]);
        delay(20);
      }
    }
  }

  //Lichtablauf balken per Array
  for (int i = 0; i < ColorNumberPcs; i++)
  {
    for(int z = 0; z < 8; z++){
        setRGBWValue(z/4+z/8, z%4, ledCollorsArray[i][0], ledCollorsArray[i][1], ledCollorsArray[i][2], ledCollorsArray[i][3]);
        setRGBWValue(z/4+z/8+2, z%4, ledCollorsArray[i][0], ledCollorsArray[i][1], ledCollorsArray[i][2], ledCollorsArray[i][3]);
        setRGBWValue(z/4+z/8+4, z%4, ledCollorsArray[i][0], ledCollorsArray[i][1], ledCollorsArray[i][2], ledCollorsArray[i][3]);
        setRGBWValue(z/4+z/8+6, z%4, ledCollorsArray[i][0], ledCollorsArray[i][1], ledCollorsArray[i][2], ledCollorsArray[i][3]);
        setRGBWValue(z/4+z/8+8, z%4, ledCollorsArray[i][0], ledCollorsArray[i][1], ledCollorsArray[i][2], ledCollorsArray[i][3]);
        delay(50);
    }
  }
  
  //Lichtablauf einzeln mit Dimmfunktion
  for (int i = 0; i < 8; i++)
  {
    setRGBWSlowOnOff(i, 1, 0, 1, 0, 1, 5);  // Serial port for debugging purposes
    Serial.println("Dimmenup");
  }
    
  for (int i = 0; i < 8; i++)
  {
    setRGBWSlowOnOff(i, 1, 0, 1, 0, 0, 5); 
    Serial.println("Dimmendown");  
  }
  
  //Lichtablauf einzeln mit Dimmfunktion
  for (int i = 0; i < 8; i++)
  {
    setRGBWSlowOnOff(i, 1, 1, 0, 0, 1, 5);  // Serial port for debugging purposes
    setRGBWSlowOnOff(i, 1, 1, 0, 0, 0, 5);
  }
  
  //Lichtablauf einzeln mit Dimmfunktion
  for (int i = 0; i < 8; i++)
  {
    setRGBWSlowOnOff(i, 0, 0, 0, 1, 1, 1);  // Serial port for debugging purposes
    setRGBWSlowOnOff(i, 0, 0, 0, 1, 0, 1);
  }
}

/// @brief Setzt alle LED's aktiv oder inaktiv
/// @param enable TRUE = LED --> ein  FALSE = LED --> aus
void enableLED(bool enable){
  digitalWrite(PIN_OE, !enable);
};

/// @brief Löscht alle PWM Werte auf einem Treiber
void clearAllLed(){
  
  for (int z = 0; z < LEDMODULES; z++)
  {
    for (int i = 0; i < LEDPCS; i++)
    {
      PCA[z].setPWM(i*4 + LEDR, 0, 0);
      PCA[z].setPWM(i*4 + LEDG, 0, 0);
      PCA[z].setPWM(i*4 + LEDB, 0, 0);
      PCA[z].setPWM(i*4 + LEDW, 0, 0);
    }
  }
};

/// @brief Schreibt angegebene Farbwerte auf LED  
/// @param led  LED Nr des Modules
/// @param ledR Helligkeitswert 0...4095 rot
/// @param ledG Helligkeitswert 0...4095 grün
/// @param ledB Helligkeitswert 0...4095 blau
/// @param ledW Helligkeitswert 0...4095 weiss
void setRGBWValue(u_int8_t led, u_int16_t ledR, u_int16_t ledG, u_int16_t ledB, u_int16_t ledW){
  for (int z = 0; z < LEDMODULES; z++)
  {
    if(led < 4){
    PCA[z].setPWM(led*4 + LEDR, 0, ledR);
    PCA[z].setPWM(led*4 + LEDG, 0, ledG);
    PCA[z].setPWM(led*4 + LEDB, 0, ledB);
    PCA[z].setPWM(led*4 + LEDW, 0, ledW);
    }else{
    PCA[4+z].setPWM(led*4 + LEDR, 0, ledR);
    PCA[4+z].setPWM(led*4 + LEDG, 0, ledG);
    PCA[4+z].setPWM(led*4 + LEDB, 0, ledB);
    PCA[4+z].setPWM(led*4 + LEDW, 0, ledW);

    }
  }
};

/// @brief Schreibt angegebene Farbwerte auf LED  
/// @param led  LED Nr des Modules
/// @param ledR Helligkeitswert 0...4095 rot
/// @param ledG Helligkeitswert 0...4095 grün
/// @param ledB Helligkeitswert 0...4095 blau
/// @param ledW Helligkeitswert 0...4095 weiss
void setRGBWValueBar(u_int8_t row, u_int16_t ledR, u_int16_t ledG, u_int16_t ledB, u_int16_t ledW){
  if(row < 4){
    for (int z = 0; z < 10; z=z+2)
      {
      PCA[z].setPWM((row%4)*4 + LEDR, 0, ledR);
      PCA[z].setPWM((row%4)*4 + LEDG, 0, ledG);
      PCA[z].setPWM((row%4)*4 + LEDB, 0, ledB);
      PCA[z].setPWM((row%4)*4 + LEDW, 0, ledW);
      }
  }else{
    for (int z = 1; z < 10; z=z+2)
      {
      PCA[z].setPWM((row%4)*4 + LEDR, 0, ledR);
      PCA[z].setPWM((row%4)*4 + LEDG, 0, ledG);
      PCA[z].setPWM((row%4)*4 + LEDB, 0, ledB);
      PCA[z].setPWM((row%4)*4 + LEDW, 0, ledW);
      }

    }
  //}
};

/// @brief Schreibt angegebene Farbwerte auf LED  
/// @param led  LED Nr des Modules
/// @param ledR Helligkeitswert 0...4095 rot
/// @param ledG Helligkeitswert 0...4095 grün
/// @param ledB Helligkeitswert 0...4095 blau
/// @param ledW Helligkeitswert 0...4095 weiss
void setRGBWValue(u_int16_t element, u_int8_t led, u_int16_t ledR, u_int16_t ledG, u_int16_t ledB, u_int16_t ledW){

    PCA[element].setPWM(led*4 + LEDR, 0, ledR);
    PCA[element].setPWM(led*4 + LEDG, 0, ledG);
    PCA[element].setPWM(led*4 + LEDB, 0, ledB);
    PCA[element].setPWM(led*4 + LEDW, 0, ledW);

};


void setRGBWSlowOnOff(u_int8_t led, bool ledR, bool ledG, bool ledB, bool ledW, bool on, u_int16_t speed){
  int pwmValue = 0;
  for (int i = 0; i < 100; i++)
  { 
    pwmValue = on ? sinLUT[i] : sinLUT[99-i];
    //setRGBWValue(led, (ledR ? pwmValue : 0), (ledG ? pwmValue : 0), (ledB ? pwmValue : 0), (ledW ? pwmValue : 0));
    setRGBWValueBar(led, (ledR ? pwmValue : 0), (ledG ? pwmValue : 0), (ledB ? pwmValue : 0), (ledW ? pwmValue : 0));
    delay(speed);
  }
}

/// @brief Schaltet jede LED ein und anschliessend wieder aus.  
void checkHardware(){
  for (int i = 0; i < LEDPCS; i++)
  {
    setRGBWValue(i, 4095, 0, 0, 0);
    delay(200);
    setRGBWValue(i, 0, 4095, 0, 0);
    delay(200);
    setRGBWValue(i, 0, 0, 4095, 0);
    delay(200);
    setRGBWValue(i, 0, 0, 0, 4095);
    delay(200);
    setRGBWValue(i, 0, 0, 0, 0);
    delay(200);
  }
};