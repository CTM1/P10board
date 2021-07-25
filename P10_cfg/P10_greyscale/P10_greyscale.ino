//https://fr.wikipedia.org/wiki/Serial_Peripheral_Interface
//

#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "SystemFont5x7.h"
#include "Arial_black_16.h"

#include "test.h"

//------------------------------------------------------------------------

extern unsigned short grey_tab[16][16]; // resolution, value, frame in bit
extern byte landscape[8 * 32]; // the raw image to print

//byte grey_frame[16][64];
byte** grey_frame;
int grey_def = 16;
byte grey_fid = 0;

void init_grey_frame() {
	grey_frame = (byte**)malloc(sizeof(byte*) * 16);
	for (int i = 0; i < 16; i++) {
		grey_frame[i] = (byte*)malloc(sizeof(byte) * 64);
	}
}

// define the 
boolean led_state(int value, byte frame, byte resolution) {
	// the value multiply by the resolution (with is < to 16) and devide by 16 because the value is between 0 and 15
	int id_val = (value * (int)resolution) >> 4; 
	return (grey_tab[resolution][id_val] & (1 << frame)) >> frame;
}

int DisplaysTotal = 1;

// une fonction qui remplis grey_frame a partir d'une image
void fill_grey_frame() {
	for (int f = 0; f < 16; f++) {
		for (int bY = 0; bY < 16; bY++) {
			for (int bX = 0; bX < 32; bX++) {
				int val = bX + (bY * 32);
				int state;
				if ((val % 2) == 0) {
					state = led_state((int)(landscape[val / 2] & 0xF), f, grey_def);
				} else {
					state = led_state((int)((landscape[val / 2] >> 4) & 0xF), f, grey_def);
				}

    			//byte panel=(bX/DMD_PIXELS_ACROSS) + (DisplaysWide*(bY/DMD_PIXELS_DOWN));
    			byte panel = 1;
    			bX=(bX % DMD_PIXELS_ACROSS) + (panel<<5);
    			bY=bY % DMD_PIXELS_DOWN;
    			//set pointer to DMD RAM byte to be modified
    			unsigned int id = bX/8 + bY*(DisplaysTotal<<2);
    			byte lookup = bPixelLookupTable[bX & 0x07];

				if (state == 0) { // May be we will need to inverse this condition
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
  // At each frame we charge the next image in the buffer
  // then the buffer is send with spi
  // and we increment the frame index

  led_module.load_image(grey_frame[grey_fid]);
  led_module.scanDisplayBySPI(); //Check incoming data from Arduino to call for interrupt.

  grey_fid = (grey_fid + 1) % grey_def;
}

void setup()
{
  // initialisation of memory with malloc
  // and initialisaiton of the value of grey_frame
  init_grey_frame();
  fill_grey_frame();

  Timer1.initialize(2000);
  Timer1.attachInterrupt(scan_module);
}

void loop() 
{
	// now ther is no need in the main loop.
	// After we will receive image by serial.
	// 
}
