/*
Basic function for GBA display
*/

/*
Initializes GBA display
*/
void disp_init();

/*
Page flipping function for mode 4.
Modifies display control and videoBuffer to opposite screens
Assumes videoBuffer is ALWAYS pointed to the undisplayed screen and maintains that property.
*/
void vid_flip();

/*
Sets the palette for the mode 4 bitmap display
Uses dma_cpy function for speed
*/
void set_palette(const unsigned short *palette);

/*
Waits for scanlinecounter to reach vblank
*/
void vsync();

/*
Pointer to the videoBuffer
*/
extern unsigned short *videoBuffer;