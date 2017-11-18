#include "display.h"
#include "color.h"
#include "dma.h"

/*
Basic function for GBA display
*/


#define REG_DISPCTL *(unsigned short *)0x4000000
#define DCNT_PAGE (1<<4)
#define MODE3 3
#define MODE4 4
#define BG2_ENABLE (1<<10)
#define SPRITE_ENABLE (1<<12)
#define DCNT_OBJ_1D (1<<6)

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006


unsigned short *videoBuffer = (unsigned short *)0x6000000;
unsigned short *bitmap_palette = (unsigned short *)0x5000000;

/*
Initializes GBA display
*/

void disp_init()
{
	//enable stuff
	REG_DISPCTL = MODE4 | BG2_ENABLE | SPRITE_ENABLE | DCNT_OBJ_1D;
	//start displaying at page 2
	REG_DISPCTL ^= DCNT_PAGE; 

	//init palette
	unsigned short default_palette[10] = {
		BLACK, WHITE, RED, GREEN
	};
	set_palette(default_palette);
}

/*
Page flipping function for mode 4.
Modifies display control and videoBuffer to opposite screens
Assumes videoBuffer is ALWAYS pointed to the undisplayed screen and maintains that property.
*/
void vid_flip()
{
	REG_DISPCTL ^= DCNT_PAGE; 
	if (videoBuffer <= (unsigned short *)0x6000000) {
		videoBuffer = (unsigned short *)0x600A000;
	} else {
		videoBuffer = (unsigned short *)0x6000000;
	}
}

/*
Sets the palette for the mode 4 bitmap display
Uses dma_cpy function for speed
*/
void set_palette(const unsigned short *palette)
{
	dma_cpy(palette, bitmap_palette, 256);
}

/*
Waits for scanlinecounter to reach vblank
*/
void vsync()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 161);
}