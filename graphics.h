/*
Contains basic functions for drawing graphics on screen
*/

#define WIDTH 240
#define HEIGHT 160
//number of bytes on a screen in mode 4
#define MODE4_SCREEN_SIZE (WIDTH * HEIGHT / 2)

/*
Draws a rectangle given dimensions and color in mode 4
*/
void drawRectangle(int row, int col, int width, int height, unsigned char color);

/*
Draws a character from toncFont at a given location and color.
Any odd col position will be floored due to the addressibility of GBA
*/
void drawChar(int row, int col, char ch, unsigned char color);

/*
Draws characters from a string at a given location and color.
Any odd col position will be floored due to the addressibility of GBA
*/
void drawStr(int row, int col, char *str, unsigned char color);

/*
Draws an image using DMA at the given location given its size in mode4
*/
void drawImage(int row, int col, int width, int height, const unsigned short *image);
