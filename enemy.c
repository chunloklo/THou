#include "enemy.h"
#include "player.h"
#include "sprite.h"
#include "graphics.h"
#include "img_sprite.h"
#include <stdio.h>
#include "collision.h"
#include <stdlib.h>
#include "posprintf.h"
#include "img_sprite.h"

/*
Contains function to initialize, spawn, destroy, and update enemy for both the game logic and display
Currently supports two types of enemies - Round enemies and Direct enemies.
Also contains logic for enemy -> player_bullet collision.
*/

#define MAX_PROJ 9

/*
Struct to contain data for any projectiles
Slow_factor avoids GBA's limits on floating point numbers. Actual speed = speed/slow_factor
Each Projectile also contains reference to their own sprite representation
*/
typedef struct bullet {

	int exist;
	int r;
	int c; 
	int speedr;
	int speedc;
	int size;
	int slow_factor_r;
	int slow_factor_c;
	volatile OBJ_ATTR *sprite;

} Projectile;

/*
Struct to contain data for any enemy
Actual speed = speed/slow_factor
Each enemy also contains reference to their own sprite representation.
Fire_delay determines the time between each of enemy's shots.
Each enemy also contains a frame_counter to allow them to act independently from the game's frame count
*/
typedef struct enemy {
	int exist;
	int r;
	int c; 
	int speedr;
	int speedc;
	int height;
	int width;
	int health;
	int frame_counter;
	int num_proj;
	int slow_factor_r;
	int slow_factor_c;
	int fire_delay;
	int max_proj;
	Projectile *proj;
	volatile OBJ_ATTR *sprite;

} Enemy;

/*
OAM can only contain 128 sprites. 
28 sprites spots are reserved for the player and its bullets.
Each enemy and their bullets has 10 sprite spots for their own sprites and their projectiles.
*/
int num_enemies = 10;
Enemy enemies[10] = {{0}};
int frame = 0;
int kill_score = 0;
int total_score = 0;

//When win is 0, game is won. This happens when no enemies nor projectiles are on the screen
int win = 1;

//the current wave that is happening
int wave_num = 0;



void enemy_fire(Enemy* enemy, int index);
void enemy_update_fire(Enemy *e, int num, int frame_counter);
void init_round_enemy(int index, int r, int c);
void init_direct_enemy(int index, int r, int c);
void init_enemy_bullet_speed(Enemy *e, int speedr, int speedc, int slow_factor_r, int slow_factor_c);

/*
Free up enemy projectile's space from dynamic allocation
Needs to be called before enemy_init() is called again to avoid memory leaks
*/
void enemy_destroy()
{
	for (int j = 0; j < num_enemies; j++)
	{	
		free(enemies[j].proj);
	}
}

/*
Initializes enemies and allocates memory space for projectiles
Sets up all enemies as Round enemies but will be initialized later.
All pointers though are pointing to correctly allocated spaces.
*/
void enemy_init() 
{	
	kill_score = 0;
	total_score = 0;
	win = 1;
	for (int j = 0; j < num_enemies; j++)
	{	
		enemies[j].exist = 0;
		// sizeof(Projectile);
		Projectile *proj = malloc((MAX_PROJ + 1) * sizeof(Projectile));//{{0}};
		enemies[j].proj = proj;
		enemies[j].sprite = &OAM[1+ MAX_BULLETS + j * (1 + MAX_PROJ)];

		Projectile *cur = enemies[j].proj;
		for(int i = 0; i < MAX_PROJ; i++, cur++)
		{
			cur->exist = 0;
			cur->r = HEIGHT;
			cur->c = WIDTH;
			cur->size = 8;
			cur->speedr = 1;
			cur->speedc = 0;
			cur->sprite = &OAM[(1 + MAX_BULLETS) + j * (1 + MAX_PROJ) + (1 + i)];
			cur->slow_factor_r = 2;
			cur->slow_factor_c = 4;
			sprite_set_attr(cur->sprite, HEIGHT, WIDTH, ROUND_BULLET_SPRITE_SHAPE, ROUND_BULLET_SPRITE_SIZE, ROUND_BULLET_ID, ROUND_BULLET_PALETTE_ID);
		}

	}

	//resets frames and wave number
	frame = 0;
	wave_num = 0;

}

/*
Spawns waves of enemy according the wave number and the number of frames that have passed
There is currently 15 waves total and they increase in difficulty as you go.
Good luck!
*/
void enemy_spawner()
{
	if (wave_num == 0 && frame > 0)
	{
		init_round_enemy(0, 10, 20);
		init_round_enemy(1, 20, WIDTH - 16 - 20);
		init_round_enemy(2, 40, 40);
		init_round_enemy(3, 50, WIDTH - 16 - 40);

		for(int i = 0; i< 4; i++)
		{
			enemies[i].max_proj = 3;
		}
		enemies[0].speedc = 1;
		enemies[2].speedc = 1;
		enemies[1].speedc = -1;
		enemies[3].speedc = -1;
		wave_num++;
		return;
	}

	if (wave_num == 1 && frame > 180)
	{
		init_direct_enemy(4, 10, 5);
		init_direct_enemy(5, 20, 20);
		init_direct_enemy(6, 40, 35);
		init_direct_enemy(7, 50, 50);
		init_direct_enemy(8, 60, 65);
		for(int i = 4; i< 9; i++)
		{
			enemies[i].max_proj = 4;
			enemies[i].fire_delay = 60;
			enemies[i].speedc = 1;
		}
		wave_num++;
		return;
	}

	if(wave_num == 2 && frame > 360)
	{
		init_round_enemy(0, 10, 20);
		init_round_enemy(1, 20, WIDTH - 16 - 20);
		init_round_enemy(2, 40, 40);
		init_round_enemy(3, 50, WIDTH - 16 - 40);

		for(int i = 0; i< 4; i++)
		{
			enemies[i].max_proj = 5;
			enemies[i].fire_delay = 60;
		}
		enemies[0].speedc = 1;
		enemies[2].speedc = 1;
		enemies[1].speedc = -1;
		enemies[3].speedc = -1;
		wave_num++;
		return;
	}

	if (wave_num == 3 && frame > 540)
	{
		init_direct_enemy(4, 10, WIDTH - 16 - 10);
		init_direct_enemy(5, 20, WIDTH - 16 - 40);
		init_direct_enemy(6, 30, WIDTH - 16 - 70);
		init_direct_enemy(7, 40, WIDTH - 16 - 100);
		init_direct_enemy(8, 50, WIDTH - 16 - 130);
		for(int i = 4; i< 9; i++)
		{
			enemies[i].max_proj = 8;
			enemies[i].fire_delay = 40;
			enemies[i].speedc = -2;
			init_enemy_bullet_speed(&enemies[i], 1, -1, 1, 1);
		}
		wave_num++;
		return;
	}

	if(wave_num == 4 && frame > 720)
	{
		init_round_enemy(0, 10, 20);
		init_round_enemy(1, 20, WIDTH - 16 - 20);
		init_round_enemy(2, 40, 40);
		init_round_enemy(3, 50, WIDTH - 16 - 40);

		for(int i = 0; i< 4; i++)
		{
			enemies[i].max_proj = 7;
		}
		enemies[0].speedc = 1;
		enemies[2].speedc = 1;
		enemies[1].speedc = -1;
		enemies[3].speedc = -1;
		wave_num++;
		return;
	}

	if (wave_num == 5 && frame > wave_num * 180)
	{
		init_direct_enemy(4, 10, 10);
		init_direct_enemy(5, 20, 20);
		init_direct_enemy(6, 30, 30);
		init_direct_enemy(7, 40, WIDTH - 16 - 40);
		init_direct_enemy(8, 50, WIDTH - 16 - 50);
		for(int i = 4; i< 9; i++)
		{
			enemies[i].max_proj = 8;
			enemies[i].fire_delay = 30;
			if(i < 7){
				enemies[i].speedc = 2;

			}
			else
			{
				enemies[i].speedc = -2;
			}
			
			init_enemy_bullet_speed(&enemies[i], 1, 0, 1, 1);
		}
		wave_num++;
		return;
	}

	if(wave_num == 6 && frame > wave_num * 180)
	{
		init_round_enemy(0, 10, 20);
		init_round_enemy(1, 20, WIDTH - 16 - 20);
		init_round_enemy(2, 40, 40);
		init_round_enemy(3, 50, WIDTH - 16 - 40);

		for(int i = 0; i< 4; i++)
		{
			enemies[i].max_proj = 6;
		}
		enemies[0].speedc = 1;
		enemies[2].speedc = 1;
		enemies[1].speedc = -1;
		enemies[3].speedc = -1;
		wave_num++;
		return;
	}

	if (wave_num == 7 && frame > 1080)
	{
		init_direct_enemy(0, 5, 10);
		init_direct_enemy(1, 25, 30);
		init_direct_enemy(2, 45, 50);
		init_direct_enemy(3, 65, 70);
		// init_direct_enemy(4, 85, 90);
		for(int i = 0; i< 4; i++)
		{
			// enemies[i].health = 20;
			enemies[i].max_proj = 9;
			enemies[i].fire_delay = 30;
			enemies[i].speedc = 2;
			init_enemy_bullet_speed(&enemies[i], 1, 0, 1, 1);
		}
		wave_num++;
		return;
	}

	if (wave_num == 8 && frame > 1180)
	{
		init_direct_enemy(4, 15, WIDTH - 20);
		init_direct_enemy(5, 35, WIDTH - 40);
		init_direct_enemy(6, 55, WIDTH - 60);
		init_direct_enemy(7, 75, WIDTH - 80);
		for(int i = 4; i< 8; i++)
		{
			// enemies[i].health = 20;
			enemies[i].max_proj = 9;
			enemies[i].fire_delay = 30;
			enemies[i].speedc = -2;
			init_enemy_bullet_speed(&enemies[i], 1, 0, 1, 1);
		}
		wave_num++;
		return;
	}

	if (wave_num == 9 && frame > 1280)
	{
		init_direct_enemy(0, 5, 10);
		init_direct_enemy(1, 25, 30);
		init_direct_enemy(2, 45, 50);
		init_direct_enemy(3, 65, 70);
		// init_direct_enemy(4, 85, 90);
		for(int i = 0; i< 4; i++)
		{
			// enemies[i].health = 20;
			enemies[i].max_proj = 9;
			enemies[i].fire_delay = 30;
			enemies[i].speedc = 2;
			init_enemy_bullet_speed(&enemies[i], 1, 0, 1, 1);
		}
		wave_num++;
		return;
	}
	
	if (wave_num == 10 && frame > 1440)
	{
		init_direct_enemy(4, 15, WIDTH - 20);
		init_direct_enemy(5, 35, WIDTH - 40);
		init_direct_enemy(6, 55, WIDTH - 60);
		init_direct_enemy(7, 75, WIDTH - 80);
		for(int i = 4; i< 8; i++)
		{
			// enemies[i].health = 20;
			enemies[i].max_proj = 9;
			enemies[i].fire_delay = 40;
			enemies[i].speedc = -2;
			init_enemy_bullet_speed(&enemies[i], 1, 0, 1, 1);
		}
		wave_num++;
		return;
	}

	if(wave_num == 11 && frame > 1440 + 160)
	{
		init_round_enemy(0, 10, 20);
		init_round_enemy(1, 20, WIDTH - 16 - 20);
		init_round_enemy(2, 40, 40);
		init_round_enemy(3, 50, WIDTH - 16 - 40);

		for(int i = 0; i< 4; i++)
		{
			enemies[i].max_proj = 6;
		}
		enemies[0].speedc = 1;
		enemies[2].speedc = 1;
		enemies[1].speedc = -1;
		enemies[3].speedc = -1;
		wave_num++;
		return;
	}

	if(wave_num == 12 && frame > 1440 + 2 * 160)
	{
		init_round_enemy(4, 10, 20);
		init_round_enemy(5, 20, WIDTH - 16 - 20);
		init_round_enemy(6, 40, 40);
		init_round_enemy(7, 50, WIDTH - 16 - 40);

		for(int i = 4; i< 8; i++)
		{
			enemies[i].max_proj = 6;
		}
		enemies[0].speedc = 1;
		enemies[2].speedc = 1;
		enemies[1].speedc = -1;
		enemies[3].speedc = -1;
		wave_num++;
		return;
	}

	if(wave_num == 13 && frame > 1440 + 3 * 160)
	{
		init_round_enemy(0, 10, 20);
		init_round_enemy(1, 20, WIDTH - 16 - 20);
		init_round_enemy(2, 40, 40);
		init_round_enemy(3, 50, WIDTH - 16 - 40);

		for(int i = 0; i< 4; i++)
		{
			enemies[i].max_proj = 6;
		}
		enemies[0].speedc = 1;
		enemies[2].speedc = 1;
		enemies[1].speedc = -1;
		enemies[3].speedc = -1;
		wave_num++;
		return;
	}

	if(wave_num == 14 && frame > 1440 + 4 * 160)
	{
		init_round_enemy(4, 10, 20);
		init_round_enemy(5, 20, WIDTH - 16 - 20);
		init_round_enemy(6, 40, 40);
		init_round_enemy(7, 50, WIDTH - 16 - 40);

		for(int i = 4; i< 8; i++)
		{
			enemies[i].max_proj = 6;
		}
		enemies[0].speedc = 1;
		enemies[2].speedc = 1;
		enemies[1].speedc = -1;
		enemies[3].speedc = -1;
		wave_num++;
		return;
	}

}

/*
Initializes the Round enemy at a certain enemies[] index and at position (r, c)
Their projectile normally spreads out in an arc going in all directions at a slow speed
They also move slowly and make good targets
Any overlap initialization will overrite each other.
*/
void init_round_enemy(int index, int r, int c)
{	
	Enemy *em = &enemies[index];
	em->exist = 1;
	em->r = r;
	em->c = c;
	em->speedr = 0;
	em->speedc = 1;
	em->height = 16;
	em->width = 16;
	em->health = 10;
	em->frame_counter = 0;
	em->num_proj = 0;
	em->slow_factor_r = 2;
	em->slow_factor_c = 2;
	em->fire_delay = 1;
	//idk why I need +1 space hmm. 0 terminated?

	sprite_set_attr(em->sprite, 10, 10, ROUND_ENEMY_SPRITE_SHAPE, ROUND_ENEMY_SPRITE_SIZE, ROUND_ENEMY_ID, ROUND_ENEMY_PALETTE_ID);

	volatile Projectile *cur = em->proj;
	for(int i = 0; i < MAX_PROJ; i++, cur++)
	{
		cur->exist = 0;
		cur->r = HEIGHT;
		cur->c = WIDTH;
		cur->size = 8;
		cur->speedr = 1;
		cur->speedc = -2 + i;
		cur->slow_factor_r = 2;
		cur->slow_factor_c = 4;
		sprite_set_attr(cur->sprite, HEIGHT, WIDTH, ROUND_BULLET_SPRITE_SHAPE, ROUND_BULLET_SPRITE_SIZE, ROUND_BULLET_ID, ROUND_BULLET_PALETTE_ID);
	}
}

/*
Initializes the Direct enemy at a certain enemies[] index and at position (r, c)
Their projectile normally travels in parallel straight lines 
They are small and move generally pretty quickly, making them difficult to kill.
Any overlap initialization will overrite each other.
*/
void init_direct_enemy(int index, int r, int c)
{
	Enemy *em = &enemies[index];
	em->exist = 1;
	em->r = r;
	em->c = c;
	em->speedr = 0;
	em->speedc = 1;
	if (index == 1 || index == 4)
	{
		em->speedc = -1;
	}
	em->height = 16;
	em->width = 16;
	em->health = 10;
	em->frame_counter = 0;
	em->num_proj = 0;
	em->slow_factor_r = 2;
	em->slow_factor_c = 2;
	em->fire_delay = 40;
	//idk why I need +1 space hmm. 0 terminated?

	sprite_set_attr(em->sprite, 10, 10, DIRECT_ENEMY_SPRITE_SHAPE, DIRECT_ENEMY_SPRITE_SIZE, DIRECT_ENEMY_ID, DIRECT_ENEMY_PALETTE_ID);

	volatile Projectile *cur = em->proj;
	for(int i = 0; i < MAX_PROJ; i++, cur++)
	{
		cur->exist = 0;
		cur->r = HEIGHT;
		cur->c = WIDTH;
		cur->size = 4;
		cur->speedr = 2;
		cur->speedc = 2;
		cur->slow_factor_r = 2;
		cur->slow_factor_c = 4;
		sprite_set_attr(cur->sprite, HEIGHT, WIDTH, NORMAL_BULLET_SPRITE_SHAPE, NORMAL_BULLET_SPRITE_SIZE, NORMAL_BULLET_ID, NORMAL_BULLET_PALETTE_ID);
	}
}

/*
Initializes/modifies the projectile speed of a single enemy
*/
void init_enemy_bullet_speed(Enemy *e, int speedr, int speedc, int slow_factor_r, int slow_factor_c)
{	
	volatile Projectile *proj = e->proj;

	for (int i = 0; i < e->max_proj; i++, proj++)
	{	
		proj->speedr = speedr;
		proj->speedc = speedc;
		proj->slow_factor_r = slow_factor_r;
		proj->slow_factor_c = slow_factor_c;
	}
}

/*
Updates enemies based on their speed and health
Handles projectile firing based on fire_delay of each enemy
Also determines whether win conditions (no enemies and projectiles) are met.
Updates scores also based on kills.
*/
void enemy_update()
{
	win = 0;
	//increment frames
	frame++;
	Enemy *cur = &enemies[0];
	for (int i = 0; i < num_enemies -1; i++, cur++)
	{	
		//update enemy if exist
		if (cur->exist)
		{	
			win++;
			if (!(frame % cur->slow_factor_r))
			{
				cur->r = cur->r + (cur->speedr);
			
			}
			if (!(frame % cur->slow_factor_c))
			{
				cur->c += (cur->speedc);
			}

			//destroy check
			if (cur->health  <= 0)
			{
				kill_score += 500;
				cur->r = HEIGHT;
				cur->c = WIDTH;
			}

			//out of bounds check
			if (cur->r + cur->height > HEIGHT || cur->r < 0 || cur->c < 0 || cur->c + cur->width > WIDTH)
			{
				cur->exist = 0;
				cur->r = HEIGHT;
				cur->c = WIDTH;
				sprite_remove(cur->sprite);
			}

			//fire projectile if possible
			if (!(cur->frame_counter % cur->fire_delay) && cur->num_proj < cur->max_proj)
			{
				enemy_fire(cur, cur->num_proj);
				cur->num_proj += 1;
			}
			//update enemy frame counter
			cur->frame_counter += 1;

			
		}
		//update each enemy's projectiles
		enemy_update_fire(cur, MAX_PROJ, frame);

	}
	//update total score
	total_score = kill_score + frame;	

}

/*
Fires the index projectile of enemy
Each enemy only has a certain amount of projectile based on the MAX_PROJ limit
MAX_PROJ is bounded above by the sprite limit for each enemy's projectile (9)
*/
void enemy_fire(Enemy* enemy, int index)
{
	volatile Projectile *proj = &enemy->proj[index];
	proj->exist = 1;
	proj->r = enemy->r;
	proj->c = enemy->c;

}

/*
Updates projectiles of the given enemy given a frame_counter;
Num represents the number of projectiles to update starting from index 0
*/
void enemy_update_fire(Enemy *e, int num, int frame_counter)
{	
	volatile Projectile *proj = e->proj;

	for (int i = 0; i < num; i++, proj++)
	{	
		
		if(proj->exist){
			win++;

			if (player_check_collision(proj->r + proj->size, proj->c + proj->size, proj->size))
			{
				proj->r =  HEIGHT;
				proj->c =  WIDTH;
			}

			if (!(frame_counter % proj->slow_factor_r))
			{
				proj->r = proj->r + (proj->speedr);
			
			}
			if (!(frame_counter % proj->slow_factor_c))
			{
				proj->c += (proj->speedc);
			}
		
		}
	}
}


/*
Updates the sprites of all enemies and projectiles given they exist.
Called at the end for synchronous update
*/
void enemy_update_sprites()
{
	Enemy *cur = enemies;
	for (int i = 0; i < num_enemies; i++, cur++)
	{	
		//update enemy if exist
		if (cur->exist)
		{
			sprite_update_pos(cur->sprite, cur->r , cur->c);
		}
		volatile Projectile *proj = cur->proj;

		//update projectiles
		for (int i = 0; i < MAX_PROJ; i++, proj++)
		{	
			
			if (proj->r + proj->size > HEIGHT || proj->r <= 0 || proj->c <= 0 || proj->c + proj->size > WIDTH)
			{
				proj->r =  HEIGHT;
				proj->c =  WIDTH;
				sprite_update_pos(proj->sprite, proj->r, proj->c);
				proj->exist = 0;
			}
				sprite_update_pos(proj->sprite, proj->r, proj->c);



		}
	}
}


/*
Checks collision for a given rectangular object and all enemies
Returns 1 if collision is found and 0 if it is not
Mainly used for checking whether player's bullets have collided with an enemy in player.c
*/
int enemy_check_collision(int r, int c, int width, int height)
{
	Enemy *cur = enemies;
	for (int i = 0; i < num_enemies; i++, cur++)
	{	
		//update enemy if exist
		if (cur->exist && detectRectCollision(r, c, width, height, cur->r, cur->c, cur->width, cur->height))
		{
			cur->health -= 1;
			return 1;
		}
	}
	return 0;
}