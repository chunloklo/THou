#define START_LIVES 9
#define START_BOMBS 2
#define START_POWER 0
#define FIREDELAY 1
#define MAX_BULLETS 26

/*
Manages the player and the bullets that they shoot
*/

/*
Player struct that contain all relevant player information
*/
typedef struct Player {
	unsigned int lives;
	int r;
	int c;
	int size;
	unsigned int numBombs;
	unsigned int power;
	unsigned int speed;
	int speed_slow_factor;
} Player;

/*
Initializes the player
*/
void player_init();

/*
Handles player movement based on input
*/
void player_move();

/*
Creates a bullet object at the end of the bullets array
Fails to do so if max player has been reached
*/
void player_create_bullet(int r, int c);

/*
Updates all existing bullets position based on bullet speed
*/
void update_bullets();

/*
Allows player to fire based on player_fire_timer
*/
void update_fire();

/*
Initializes all of player's bullets to the corner of the map
*/
void player_bullet_init();

/*
Checks collision between the player and possible bullets
*/
int player_check_collision(int r, int c, int radius);

/*
Update player bullet sprites
*/
void player_update_sprites();

//primary player
extern Player player;