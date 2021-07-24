#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include <SystemFont5x7.h>
#include "Arial_black_16.h"
#include <string.h>

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
        flag = led_module.stepMarquee(-1, 0);
        timming = millis();
      }
    }
}

void loop()
{
  if (Serial.available() == 0)
    print_scrolling_text("https://192.168.43.118:8080", 30);

  
  if (Serial.available() > 0)
  {
    String request = Serial.readString();
    char* request_id  = request.c_str();
    char* strToken =  strtok(request_id, ",");

    switch(request_id[0])
    {
      case '0': //Scrolling text
        strToken = strtok(NULL, ",");
        char* c = strToken;
        
        for (int i = 0; i < 5; i++)
          print_scrolling_text(c, 30);
        break;
        
      case '1': //Drawing //1,12,24 //1\012\024 while !null
        strToken = strtok(request_id, ",");
        int x = 0;
        int y = 0;
        
        strToken = strtok(NULL, ",");
        x = atoi(strToken);
        strToken = strtok(NULL, ",");
        y = atoi(strToken);
        
        led_module.writePixel(x,y,GRAPHICS_NORMAL,1);
        
        break;
      default:
        break;
    }
  }
}
