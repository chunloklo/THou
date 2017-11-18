#include "dma.h"
#include "display.h"
#include "color.h"
#include "num.h"

/*
Contains functions to perform DMA operations
*/

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)


#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

/*
Struct to simplify DMA control
*/
typedef struct
{
	const volatile void *src;
	volatile void *dst;
	unsigned int cnt;
} DMA_CONTROLLER;

/*
Fills current screen with the specified color in the color palette
Only fills the portion of the screen that the videoBuffer is pointing to.
*/
void dma_fill_screen(u8 color)
{
	u16 fill_color = (u16)color << 8 | color;
	DMA[3].src = &fill_color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (160*240 / 2) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;

}

/*
Generic copy function for DMA to copy element with specified size.
*/
void dma_cpy(const void *src, void *dst, int size)
{
	DMA[3].src = src;
	DMA[3].dst = dst;
	DMA[3].cnt = size | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
}
