#include <stdio.h>
#include "posprintf.h"

#include "input.h"
#include "color.h"
#include "player.h"
#include "num.h"
#include "graphics.h"
#include "dma.h"
#include "display.h"
#include "sprite.h"
#include "enemy.h"

#include "img_sprite.h"
#include "images.h"

/*
Contains the main logic flow for the different game states and switches between them
*/


//Game state
enum State{
	START,
	GAME,
	OVER,
	WIN,
};


//Display texts
char start_text[] = "Press Start";
char lose_text[] = "You Lose!";
char restart_text[] = "Press Start to Retry!";
char win_text[] = "You Win!";
char score_text[] = "Score:";

/*
Main game loop
*/

int main()
{
	
	//Initialize display and sprites
	disp_init();
	sprite_init();
	
	enum State state = START;

	while(1)
	{
		switch(state) {

		case (START):

			vsync();
			set_palette(images_palette);
			dma_cpy(start_screen, videoBuffer, START_SCREEN_SIZE / 2);

			//Start!
			drawStr(HEIGHT - 8, 0, start_text, 0);
			

			if(KEY_DOWN_NOW(BUTTON_START)) {
				//transition to gameplay
				state = GAME;
				dma_fill_screen(10);
				vid_flip();

				player_init();
				player_bullet_init();
				enemy_init();

			}

			break;

		case (GAME):;
			//clear screen
			dma_fill_screen(10);

			//print kill score
			char score_num[10] = "";
			posprintf(score_num, "%d", total_score);
			drawStr(HEIGHT - 16, 0, score_num, 0);

			//heart UI
			drawImage(HEIGHT - 8, 0, PIXELHEART_WIDTH, PIXELHEART_HEIGHT, pixelHeart);

			//lives UI
			char str1[2] = "";
			str1[0] = player.lives + 48; 
			str1[1] = '\0';
			drawStr(HEIGHT - 8, 10, str1, 0);

			//set player sprite position
			OAM[0].attr0 = (player.r - 4) | PLAYER_SPRITE_SHAPE;
			OAM[0].attr1 = (player.c - 4) | PLAYER_SPRITE_SIZE;
			OAM[0].attr2 = 512 | PLAYER_ID | PLAYER_PALETTE_ID;

			enemy_spawner();
			enemy_update();

			player_move();
			update_fire();

			update_bullets();

			if(KEY_DOWN_NOW(BUTTON_SELECT))
			{
				state = START;
				sprite_hide();
				enemy_destroy();
				break;
			}

			if(player.lives <= 0)
			{
				state = OVER;
				sprite_hide();
				enemy_destroy();
				
				break;
			}

			if(!win && wave_num > 13)
			{	
				state = WIN;
				sprite_hide();
				enemy_destroy();
				
				break;
			}

			//GRAPHICS
			vsync();
			player_update_sprites();
			enemy_update_sprites();
			
			break;

		case (OVER):
			//You Lose!
			vsync();
			dma_cpy(lose_screen, videoBuffer, START_SCREEN_SIZE / 2);
			drawStr(HEIGHT - 8, 0, restart_text, 0);
			drawStr(0, 0, lose_text, 0);

			//Score:total_score
			drawStr(8, 0, score_text, 0);
			total_score = kill_score + frame;
			char tscore_num_over[20] = "";
			posprintf(tscore_num_over, "%d", total_score);
			drawStr(8,48, tscore_num_over, 0);

			if(KEY_DOWN_NOW(BUTTON_SELECT))
			{
				state = START;
				break;
			}

			if(KEY_DOWN_NOW(BUTTON_START))
			{
				state = GAME;
				dma_fill_screen(10);
				vid_flip();

				player_init();
				player_bullet_init();

				enemy_init();

			}
			break;
		case (WIN):
			vsync();
			dma_cpy(win_screen, videoBuffer, START_SCREEN_SIZE / 2);

			//You Win!
			drawStr(0, 0, win_text, 0);
			drawStr(0, WIDTH - 88, start_text, 0);

			//Score:total_score
			drawStr(8, 0, score_text, 0);
			total_score = kill_score + frame;
			char tscore_num[20] = "";
			posprintf(tscore_num, "%d", total_score);
			drawStr(8,48, tscore_num, 0);

			if(KEY_DOWN_NOW(BUTTON_SELECT))
			{
				state = START;
				break;
			}

			if(KEY_DOWN_NOW(BUTTON_START)) {
				state = GAME;
				dma_fill_screen(10);
				vid_flip();

				player_init();
				player_bullet_init();

				enemy_init();

			}
		} 
		
		//flip after every frame
		vid_flip();
	}
	return 0;
}