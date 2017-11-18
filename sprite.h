/*
Contains functions for initializing, destroying, moving, and changing attributes of sprites
*/

//struct definition to handle TILES and 	attributes inside it
typedef struct { unsigned int data[8];  } TILE, TILE4;
typedef TILE  CHARBLOCK[512];
#define ALIGN4 __attribute__((aligned(4)))

typedef struct tagOBJ_ATTR
{
    unsigned short attr0;
    unsigned short attr1;
    unsigned short attr2;
    short fill;
} ALIGN4 OBJ_ATTR;

//OAM address
#define OAM ((volatile OBJ_ATTR *) 0x7000000)
//Max number of sprites for GBA
#define NUM_SPRITES 128

void sprite_init();
/*
Move all sprites to the bottom right corner outside
*/
void sprite_hide();

/*
Set all of the attributes of the sprite
*/
void sprite_set_attr(volatile OBJ_ATTR *sprite, unsigned short r, unsigned short c, unsigned short shape, unsigned short size, unsigned short id, unsigned short palette_index);
void sprite_update_pos(volatile OBJ_ATTR *sprite, unsigned short r, unsigned short c);

/*
Move a single sprite to the bottom right corner
*/
void sprite_remove(volatile OBJ_ATTR *sprite);

/*
Replace the sprite information with new info
*/
void sprite_replace(volatile OBJ_ATTR *src, volatile OBJ_ATTR *dst);