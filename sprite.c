#include "sprite.h"
#include "dma.h"
#include "num.h"
#include "graphics.h"

#include "img_sprite.h"

/*
Contains functions for initializing, destroying, moving, and changing attributes of sprites
*/

//pointer to each sprite tile in memory
#define tile_mem  ((CHARBLOCK*)0x06000000)
//palette address for sprites
u16 *sprite_palette = (u16 *) 0x5000200;


void sprite_init()
{
	//load sprite into memory
	dma_cpy(img_sprite, &tile_mem[4][512], IMG_SPRITE_LENGTH);
	//load palette from sprite
	dma_cpy(img_sprite_palette, sprite_palette, IMG_SPRITE_PALETTE_LENGTH);
}

/*
Move all sprites to the bottom right corner outside
*/
void sprite_hide()
{
	volatile OBJ_ATTR *cur = OAM;
	for (int i = 0; i < NUM_SPRITES; i++)
	{
		cur->attr0 = (cur->attr0 & ~0xFF) | HEIGHT;
		cur->attr1 = (cur->attr1 & ~0xFF) | WIDTH;
		cur++;
	}
}

/*
Set all of the attributes of the sprite
*/
void sprite_set_attr(volatile OBJ_ATTR *sprite, unsigned short r, unsigned short c, unsigned short shape, unsigned short size, unsigned short id, unsigned short palette_index)
{
	sprite->attr0 = r | shape;
	sprite->attr1 = c | size;
	sprite->attr2 = (512 + id) | id | palette_index;

}

void sprite_update_pos(volatile OBJ_ATTR *sprite, unsigned short r, unsigned short c)
{
	sprite->attr0 = (sprite->attr0 & ~0xFF) | r;
	sprite->attr1 = (sprite->attr1 & ~0xFF) | c;
}

/*
Move a single sprite to the bottom right corner
*/
void sprite_remove(volatile OBJ_ATTR *sprite)
{
	sprite->attr0 = (sprite->attr0 & ~0xFF) | HEIGHT;
	sprite->attr1 = (sprite->attr1 & ~0xFF) | WIDTH;
}

/*
Replace the sprite information with new info
*/
void sprite_replace(volatile OBJ_ATTR *src, volatile OBJ_ATTR *dst)
{
	*src = *dst;
}