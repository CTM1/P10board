//https://fr.wikipedia.org/wiki/Serial_Peripheral_Interface
//

#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
  
#define ROW 1 //We're only using one module so 1 row 1 col
#define COL 1
#define FONT Arial_Black_16
DMD led_module(ROW, COL);

void scan_module()
{
  led_module.scanDisplayBySPI(); //Check incoming data from Arduino to call for interrupt.
}

void setup()
{
  Timer1.initialize(2000);
  Timer1.attachInterrupt(scan_module);}

void loop() 
{
  //led_module.drawBox(0, 0, (32 * ROW) - 1, (16 * COL) - 1, GRAPHICS_NORMAL);
  //led_module.drawCircle(16,  8,  5, GRAPHICS_NORMAL );
  
 for (byte i = 0; i < 32; i++)
 {
    //drawLine(start x, end x, start y, end y)
    led_module.drawCircle(i,  10,  2, GRAPHICS_NORMAL); //wheel 1
    led_module.drawCircle(i + 9,  10,  2, GRAPHICS_NORMAL); //wheel 2
    led_module.drawLine(i + 2, 10, 6 + i, 10, GRAPHICS_NORMAL); //floor
    led_module.drawLine(i, 8, i, 6, GRAPHICS_NORMAL); //vertical 1
    led_module.drawLine(i + 1, 6, 6 + i, 6, GRAPHICS_NORMAL); //top 1
    led_module.drawLine(7 + i, 6, 8 + i, 7, GRAPHICS_NORMAL);  //(front

    delay(15);
    led_module.clearScreen(true);
 }
 led_module.selectFont(FONT);
 led_module.drawMarquee("vroum", 5, (32 * COL) - 1, 0);
 long start = millis();
 long timer = start;
 boolean ret = false;
 while(!ret)
 {
   if (timer + 20 < millis()) 
   {
    ret = led_module.stepMarquee(-1,0);
    timer = millis();    
   }
 }
}
