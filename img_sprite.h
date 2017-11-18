/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --transparent=0 --bpp=4 --mode=sprites img_sprite direct_enemy.png normal_bullet.png player.png player_bullet.png round_bullet.png round_enemy.png 
 * Time-stamp: Sunday 11/12/2017, 08:50:33
 * 
 * Contains information for all of the sprites
 *
 * Image Information
 * -----------------
 * direct_enemy.png 16@32
 * normal_bullet.png 8@8
 * player.png 8@16
 * player_bullet.png 8@16
 * round_bullet.png 16@16
 * round_enemy.png 32@32
 * Transparent color: (0, 0, 0)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef IMG_SPRITE_H
#define IMG_SPRITE_H

#define IMG_SPRITE_PALETTE_TYPE (0 << 13)
#define IMG_SPRITE_DIMENSION_TYPE (1 << 6)

extern const unsigned short img_sprite_palette[256];
#define IMG_SPRITE_PALETTE_SIZE 512
#define IMG_SPRITE_PALETTE_LENGTH 256

extern const unsigned short img_sprite[528];
#define IMG_SPRITE_SIZE 1056
#define IMG_SPRITE_LENGTH 528

#define NORMAL_BULLET_PALETTE_ID (0 << 12)
#define NORMAL_BULLET_SPRITE_SHAPE (0 << 14)
#define NORMAL_BULLET_SPRITE_SIZE (0 << 14)
#define NORMAL_BULLET_ID 0

#define PLAYER_BULLET_PALETTE_ID (1 << 12)
#define PLAYER_BULLET_SPRITE_SHAPE (2 << 14)
#define PLAYER_BULLET_SPRITE_SIZE (0 << 14)
#define PLAYER_BULLET_ID 1

#define ROUND_BULLET_PALETTE_ID (2 << 12)
#define ROUND_BULLET_SPRITE_SHAPE (0 << 14)
#define ROUND_BULLET_SPRITE_SIZE (1 << 14)
#define ROUND_BULLET_ID 3

#define ROUND_ENEMY_PALETTE_ID (3 << 12)
#define ROUND_ENEMY_SPRITE_SHAPE (0 << 14)
#define ROUND_ENEMY_SPRITE_SIZE (2 << 14)
#define ROUND_ENEMY_ID 7

#define PLAYER_PALETTE_ID (4 << 12)
#define PLAYER_SPRITE_SHAPE (2 << 14)
#define PLAYER_SPRITE_SIZE (0 << 14)
#define PLAYER_ID 23

#define DIRECT_ENEMY_PALETTE_ID (4 << 12)
#define DIRECT_ENEMY_SPRITE_SHAPE (2 << 14)
#define DIRECT_ENEMY_SPRITE_SIZE (2 << 14)
#define DIRECT_ENEMY_ID 25

#endif

