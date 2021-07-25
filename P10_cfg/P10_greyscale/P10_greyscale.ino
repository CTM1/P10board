//https://fr.wikipedia.org/wiki/Serial_Peripheral_Interface
//

#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "SystemFont5x7.h"
#include "Arial_black_16.h"

#include "test.h"

//------------------------------------------------------------------------
extern unsigned short grey_tab[1][1]; // resolution, value, frame in bit
extern byte landscape[16 * 32]; // the raw image to print

byte grey_frame[16][64];
int grey_def = 16;
byte grey_fid = 0;

boolean led_state(int value, byte frame, byte resolution) {
	int id_val = (value * resolution) / 255;
	return (grey_tab[resolution][id_val] & (1 << frame)) >> frame;
}

//static byte bPixelLookupTable[8] =
//{
//   0x80,   //0, bit 7
//   0x40,   //1, bit 6
//   0x20,   //2. bit 5
//   0x10,   //3, bit 4
//   0x08,   //4, bit 3
//   0x04,   //5, bit 2
//   0x02,   //6, bit 1
//   0x01    //7, bit 0
//};

/*

    unsigned int data_id;

    byte panel=(bX/DMD_PIXELS_ACROSS) + (DisplaysWide*(bY/DMD_PIXELS_DOWN));
    bX=(bX % DMD_PIXELS_ACROSS) + (panel<<5);
    bY=bY % DMD_PIXELS_DOWN;
    //set pointer to DMD RAM byte to be modified
    data_id = bX/8 + bY*(DisplaysTotal<<2);

    byte lookup = bPixelLookupTable[bX & 0x07];


	bDMDScreenRAM[data_id] &= ~lookup;	// zero bit is pixel on

//*/

//int DMD_PIXELS_ACROSS = 32;
//int DisplaysWide = 1;
//int DMD_PIXELS_DOWN = 16;
int DisplaysTotal = 1;

// une fonction qui remplis grey_frame a partir d'une image
void fill_grey_frame(byte img[]) {
	for (int f = 0; f < 16; f++) {
		for (int bY = 0; bY < 16; bY++) {
			for (int bX = 0; bX < 32; bX++) {
				int val = bX + (bY * 32);
				int state = led_state((int)img[val], f, grey_def);

    			unsigned int id;
    			//byte panel=(bX/DMD_PIXELS_ACROSS) + (DisplaysWide*(bY/DMD_PIXELS_DOWN));
    			byte panel = 1;
    			bX=(bX % DMD_PIXELS_ACROSS) + (panel<<5);
    			bY=bY % DMD_PIXELS_DOWN;
    			//set pointer to DMD RAM byte to be modified
    			id = bX/8 + bY*(DisplaysTotal<<2);
    			byte lookup = bPixelLookupTable[bX & 0x07];

				if (state == 1) {
					grey_frame[f][id] |= lookup;
				} else {
					grey_frame[f][id] &= ~lookup;
				}
			}
		}
	}
}

//------------------------------------------------------------------------
  
#define ROW 1 //We're only using one module so 1 row 1 col
#define COL 1
#define FONT Arial_Black_16
DMD led_module(ROW, COL);

void scan_module()
{
  // TODO: avoir un truc qui charge une image dans le buffer
  led_module.load_image(grey_frame[grey_fid]);
  led_module.scanDisplayBySPI(); //Check incoming data from Arduino to call for interrupt.

  grey_fid = (grey_fid + 1) % grey_def;
}

void setup()
{
  Timer1.initialize(2000);
  Timer1.attachInterrupt(scan_module);
  // TODO: charger les different frame pour le grey scale en fonction d'une image enrer en dure
  fill_grey_frame(landscape);
}

void loop() 
{
//  //led_module.drawBox(0, 0, (32 * ROW) - 1, (16 * COL) - 1, GRAPHICS_NORMAL);
//  //led_module.drawCircle(16,  8,  5, GRAPHICS_NORMAL );
//  
// for (byte i = 0; i < 32; i++)
// {
//    //drawLine(start x, end x, start y, end y)
//    led_module.drawCircle(i,  10,  2, GRAPHICS_NORMAL); //wheel 1
//    led_module.drawCircle(i + 9,  10,  2, GRAPHICS_NORMAL); //wheel 2
//    led_module.drawLine(i + 2, 10, 6 + i, 10, GRAPHICS_NORMAL); //floor
//    led_module.drawLine(i, 8, i, 6, GRAPHICS_NORMAL); //vertical 1
//    led_module.drawLine(i + 1, 6, 6 + i, 6, GRAPHICS_NORMAL); //top 1
//    led_module.drawLine(7 + i, 6, 8 + i, 7, GRAPHICS_NORMAL);  //(front
//
//    delay(15);
//    led_module.clearScreen(true);
// }
// led_module.selectFont(FONT);
// led_module.drawMarquee("vroum", 5, (32 * COL) - 1, 0);
// long start = millis();
// long timer = start;
// boolean ret = false;
// while(!ret)
// {
//   if (timer + 20 < millis()) 
//   {
//    ret = led_module.stepMarquee(-1,0);
//    timer = millis();    
//   }
// }
}
