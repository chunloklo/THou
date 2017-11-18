/*
 * Exported with nin10kit v1.7
 * Time-stamp: Sunday 11/12/2017, 10:34:21
 *
 * Contains all images displayed
 * 
 * Image Information
 * -----------------
 * /home/innovation/Downloads/screens/lose_screen.png 240@160
 * /home/innovation/Downloads/screens/pixelHeart.png 9@9
 * /home/innovation/Downloads/screens/start_screen.png 240@160
 * /home/innovation/Downloads/screens/win_screen.png 240@160
 * /home/innovation/Downloads/screens/lose_screen.png 240@160
 * /home/innovation/Downloads/screens/pixelHeart.png 8@8
 * /home/innovation/Downloads/screens/start_screen.png 240@160
 * /home/innovation/Downloads/screens/win_screen.png 240@160
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef IMAGES_H
#define IMAGES_H

extern const unsigned short images_palette[256];
#define IMAGES_PALETTE_SIZE 512
#define IMAGES_PALETTE_LENGTH 256

extern const unsigned short lose_screen[19200];
#define LOSE_SCREEN_SIZE 38400
#define LOSE_SCREEN_LENGTH 19200
#define LOSE_SCREEN_WIDTH 240
#define LOSE_SCREEN_HEIGHT 160

extern const unsigned short pixelHeart[32];
#define PIXELHEART_SIZE 64
#define PIXELHEART_LENGTH 32
#define PIXELHEART_WIDTH 8
#define PIXELHEART_HEIGHT 8

extern const unsigned short start_screen[19200];
#define START_SCREEN_SIZE 38400
#define START_SCREEN_LENGTH 19200
#define START_SCREEN_WIDTH 240
#define START_SCREEN_HEIGHT 160

extern const unsigned short win_screen[19200];
#define WIN_SCREEN_SIZE 38400
#define WIN_SCREEN_LENGTH 19200
#define WIN_SCREEN_WIDTH 240
#define WIN_SCREEN_HEIGHT 160

#endif

