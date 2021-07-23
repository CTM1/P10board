#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include <SystemFont5x7.h>
#include "Arial_black_16.h"

#define ROW 1
#define COL 1
#define FONT Arial_Black_16
DMD led_module(ROW, COL);

void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Timer1.initialize(2000);
  Timer1.attachInterrupt(scan_module);
}

void scan_module()
{
  led_module.scanDisplayBySPI(); //Check incoming data from Arduino to call for interrupt.
}

void loop()
{
  if (Serial.available() > 0)
  {
    String inByte = Serial.readString();
    char* c = inByte.c_str();
    
    //Serial.write(inByte);

    led_module.clearScreen(true);
    led_module.selectFont(FONT);
    led_module.drawMarquee(c, strlen(c), (32 * ROW), 0);  // void drawMarquee( const char* bChars, byte length, int left, int top, byte fgcolour, byte bgcolour);

    long start = millis();
    long timming = start;
    boolean flag = false;
    while (!flag)
    {
      if ((timming + 50) < millis()) 
      {
        flag = led_module.stepMarquee(-1, 0);
        timming = millis();
      }
    }
  }
}
