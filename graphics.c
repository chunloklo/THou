#include "graphics.h"
#include "display.h"
#include "num.h"
#include "dma.h"

/*
Contains basic functions for drawing graphics on screen
*/

#define OFFSET(row, col, rowlen)  ((row)*(rowlen)+(col))

//imports font done by tonc
extern const unsigned int toncfontTiles[192];

/*
Draws a rectangle given dimensions and color in mode 4
*/
void drawRectangle(int row, int col, int width, int height, unsigned char color)
{
	int fullColor = (int) color | (color << 8);
	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c = c + 2)
		{
			u16 *dst = &videoBuffer[OFFSET(row + r, col + c, WIDTH) / 2];
			if (col & 1 && c == 0)
			{
				*dst = (*dst& 0xFF) | ((color - 1) << 8);
				c--;
			}
			else if (c + 1 >= width)
			{
				*dst = (*dst& ~0xFF) | (color - 1);
			}
			else
			{
				*dst = fullColor;
			}
			
		}
	}
}

/*
Draws a character from toncFont at a given location and color.
Any odd col position will be floored due to the addressibility of GBA
*/
void drawChar(int row, int col, char ch, unsigned char color)
{
	//create character pointer
	u8 *fontptr = (u8 *)&toncfontTiles[(ch - 32) * 2];
	for (int r = 0; r < 8; r++)
	{
		u8 line = fontptr[r];
		for(int c = 0; c < 8; c += 2)
		{
			u16 *dst = &videoBuffer[OFFSET(row + r, col + c, WIDTH) / 2];

			if (line % 4 == 3)
			{
				*dst = color + (color << 8);
			}
			else if (line % 4 == 2)
			{
				*dst = (*dst & 0xFF) | (color << 8);
			}
			else if (line % 4 == 1)
			{
				*dst = (*dst & ~0xFF) | color;
			}
			line = line >> 2;


		}
	}

}

/*
Draws characters from a string at a given location and color.
Any odd col position will be floored due to the addressibility of GBA
*/
void drawStr(int row, int col, char *str, unsigned char color)
{
	int charNum = 0;
	while(*str != 0) {
		drawChar(row, col + charNum * 8, *str, color);
		str++;
		charNum++;
	}
}

/*
Draws an image using DMA at the given location given its size.
*/
void drawImage(int row, int col, int width, int height, const unsigned short *image)
{
	for (int r = 0; r < height; r++)
	{
		dma_cpy(&image[r * width / 2], &videoBuffer[(row+ r) * 120 + col], width/2);
	}
}