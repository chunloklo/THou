#include "player.h"
#include "input.h"
#include "graphics.h"
#include "sprite.h"
#include "img_sprite.h"
#include "stdio.h"
#include "enemy.h"
#include "collision.h"

/*
Manages the player and the bullets that they shoot
*/

//The player
Player player;


/*
Struct to organize player's bullets with its attributes and sprite position
*/
typedef struct bullet {

	int exist;
	int r;
	int c; 
	int speed;
	int size;
	volatile OBJ_ATTR *sprite;

} Bullet;

//Array that contains all player bullet information
Bullet pbullets[MAX_BULLETS] = {{0}};
//Keep tracks of the size of the array
int pbullets_size = 0;

/*
Diagram for how size relates to the player coordinate (center)
    size
	 |
size[ ]size
	 |
    size
	 |
*/

/*
Initializes the player
*/
void player_init()
{
	player.lives = START_LIVES;
	player.numBombs = START_BOMBS;
	player.power = START_POWER;
	player.speed = 2;
	player.size = 3;
	player.r = HEIGHT - 8;
	player.c = WIDTH / 2;
	player.speed_slow_factor = 1;
}

/*
Handles player movement based on input
*/
void player_move()
{
	if (frame % player.speed_slow_factor)
	{
		return;
	}
	if (KEY_DOWN_NOW(BUTTON_B))
	{
		// player.speed = 2;
		player.speed_slow_factor = 3;

	}
	else
	{
		player.speed = 2;
		player.speed_slow_factor = 1;
	}

	if(KEY_DOWN_NOW(BUTTON_RIGHT))
	{
		player.c += player.speed;
		if (player.c >= WIDTH - 4)
		{
			player.c = WIDTH - 4;
		}
	}
	if(KEY_DOWN_NOW(BUTTON_LEFT))
	{
		player.c -= player.speed;
		if (player.c <= 4)
		{
			player.c = 4;
		}
	}
	if(KEY_DOWN_NOW(BUTTON_UP))
	{
		player.r -= player.speed;
		if (player.r <= 5)
		{
			player.r = 5;
		}
	}
	if(KEY_DOWN_NOW(BUTTON_DOWN))
	{
		player.r += player.speed;
		if (player.r >= HEIGHT - 8)
		{
			player.r = HEIGHT - 8;
		}
	}
}

/*
Initializes all of player's bullets to the corner of the map
*/
void player_bullet_init()
{
	volatile OBJ_ATTR *cur = &OAM[1];
	for (int i = 0; i < MAX_BULLETS; i++, cur++)
	{
		pbullets[i].exist = 0;
		pbullets[i].r = HEIGHT;
		pbullets[i].c = WIDTH;
		sprite_update_pos(pbullets[i].sprite, HEIGHT , WIDTH);
		sprite_set_attr(cur, HEIGHT, WIDTH, PLAYER_BULLET_SPRITE_SHAPE, PLAYER_BULLET_SPRITE_SIZE, PLAYER_BULLET_ID, PLAYER_BULLET_PALETTE_ID);
	}
	pbullets_size = 0;
}

/*
Creates a bullet object at the end of the bullets array
Fails to do so if max player has been reached
*/
void player_create_bullet(int r, int c)
{
	if (pbullets_size >= MAX_BULLETS){
		return;
	}


	pbullets[pbullets_size].exist = 1;
	pbullets[pbullets_size].r = r;
	pbullets[pbullets_size].c = c;
	pbullets[pbullets_size].size = 8;
	pbullets[pbullets_size].speed = 10;
	pbullets[pbullets_size].sprite = &OAM[1 + pbullets_size];
	sprite_update_pos(pbullets[pbullets_size].sprite, r , c);
	pbullets_size++;
}

/*
Updates all existing bullets position based on bullet speed
*/
void update_bullets()
{
	int i = 0;
	Bullet *cur = pbullets;

	while(cur->exist)
	{
		cur->r = cur->r - cur->speed;
		if (cur->r <= 0 || enemy_check_collision(cur->r, cur->c, cur->size, cur->size)) {
			//change sprite index to replace sprite
			sprite_replace(cur->sprite, pbullets[pbullets_size - 1].sprite);
			sprite_remove(pbullets[pbullets_size - 1].sprite);

			//set pbullet's sprite pointer to cur.sprite
			pbullets[pbullets_size - 1].sprite = cur->sprite; 

			//set deferenced value of cur to last element of pbullet
			*cur = pbullets[pbullets_size - 1];

			pbullets[pbullets_size - 1].exist = 0;
			pbullets_size--;
		}
		i++;
		cur++;
	}
}

/*
Delay between each players
*/
static int player_fire_timer = 0;

/*
Allows player to fire based on player_fire_timer
*/
void update_fire()
{
	if (KEY_DOWN_NOW(BUTTON_A) && player_fire_timer <= 0)
	{	
		player_create_bullet(player.r - 10, player.c - player.size);
		player_fire_timer = FIREDELAY;	
	}
	else if (player_fire_timer > 0)
	{
		player_fire_timer -= 1;
	}
	
}

/*
Checks collision between the player and possible bullets
*/
int player_check_collision(int r, int c, int radius)
{
	//update enemy if exist
	if (detectCircleCollision(player.r, player.c, player.size, r, c, radius))
	{
		player.lives -= 1;
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
Update player bullet sprites
*/
void player_update_sprites()
{

	int i = 0;
	Bullet *cur = pbullets;

	while(cur->exist)
	{

		sprite_update_pos(cur->sprite, cur->r, cur->c);
		i++;
		cur++;
	}
}
