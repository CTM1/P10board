#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include <SystemFont5x7.h>
#include "Arial_black_16.h"
#include <string.h>

#define ROW 1
#define COL 1
#define FONT Arial_Black_16
#define BUFFER_SIZE 32

DMD led_module(ROW, COL);

int times_draw = 0;
boolean drawing = false;

void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(2000000);
  Timer1.initialize(2000);
  Timer1.attachInterrupt(scan_module);
}

void scan_module()
{
  led_module.scanDisplayBySPI(); //Check incoming data from Arduino to call for interrupt.
}

void print_scrolling_text(char* c, int speed)
{
    led_module.clearScreen(true);
    led_module.selectFont(FONT);
    led_module.drawMarquee(c, strlen(c), (32 * ROW), 0);  // void drawMarquee( const char* bChars, byte length, int left, int top, byte fgcolour, byte bgcolour);

    long start = millis();
    long timming = start;
    boolean flag = false;
    while (!flag && Serial.available() ==  0)
    {
      if ((timming + speed) < millis()) 
      {
        times_draw++;
        flag = led_module.stepMarquee(-1, 0);
        timming = millis();
      }
    }
}
/*void serialEvent()
{
  char inChar = Serial.read();
}*/

void loop()
{
  if (!Serial.available())
    print_scrolling_text("http://192.168.43.118:8080", 30);
    
  if (Serial.available())
  {
    if (!drawing)
    {
      int request;
      digitalWrite(13, HIGH);
      request = Serial.read();
      Serial.println(request);
    
      if (request == 0)
      {
        drawing = false;
        int i = 0;
        char* str;
        while (i < BUFFER_SIZE)
        {
          str[i] = (char) Serial.read();
          i++;
        }
     print_scrolling_text(str, 30);
      }  
   }   
   else
   {
      
      drawing = true;
      int int_values[5] = {0, 0, 0, 0, 0};

      for (int i = 1; i < 5; i++)
        int_values[i] = Serial.read();

      if (int_values[0] == 0)
        drawing = false;
        
      if (times_draw != 0)
      {
        led_module.clearScreen(true);
        times_draw = 0;
      }
     // Serial.println(int_values[3]);
    //  Serial.print(int_values[4]);
      //Serial.println();
      led_module.writePixel(int_values[3],int_values[4], GRAPHICS_NORMAL, 1);
    } 
    digitalWrite(13, HIGH);
   }
  }
