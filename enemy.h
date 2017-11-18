/*
Contains function to initialize, spawn, destroy, and update enemy for both the game logic and display
Currently supports two types of enemies - Round enemies and Direct enemies.
Also contains logic for enemy -> player_bullet collision.
*/

/*
Initializes enemies and allocates memory space for projectiles
Sets up all enemies as Round enemies but will be initialized later.
All pointers though are pointing to correctly allocated spaces.
*/
void enemy_init();

/*
Free up enemy projectile's space from dynamic allocation
Needs to be called before enemy_init() is called again to avoid memory leaks
*/
void enemy_destroy();

/*
Updates enemies based on their speed and health
Handles projectile firing based on fire_delay of each enemy
Also determines whether win conditions (no enemies and projectiles) are met.
Updates scores also based on kills.
*/
void enemy_update();

/*
Updates the sprites of all enemies and projectiles given they exist.
Called at the end for synchronous update
*/
void enemy_update_sprites();

/*
Checks collision for a given rectangular object and all enemies
Returns 1 if collision is found and 0 if it is not
Mainly used for checking whether player's bullets have collided with an enemy in player.c
*/
int enemy_check_collision(int r, int c, int width, int height);

/*
Spawns waves of enemy according the wave number and the number of frames that have passed
There is currently 15 waves total and they increase in difficulty as you go.
Good luck!
*/
void enemy_spawner();

//frame num since gameplay started
extern int frame;
//total score
extern int total_score;
//score purely based on player kills
extern int kill_score;
//whether player has met winning condition. 0 if true, 1 if false
extern int win;
//the wave number currently
extern int wave_num;