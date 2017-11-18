/*
Contains functions to perform DMA operations
*/

/*
Fills current screen with the specified color in the color palette
Only fills the portion of the screen that the videoBuffer is pointing to.
*/
void dma_fill_screen(unsigned char color);

/*
Generic copy function for DMA to copy element with specified size.
*/
void dma_cpy(const void *src, void *dst, int size);