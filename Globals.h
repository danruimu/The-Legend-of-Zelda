
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gl/glut.h>

#define TILE_SIZE		16
#define BLOCK_SIZE		32

#define TEXTURES_WIDTH	18	//how many textures are in a row of the sprite
#define TEXTURES_HEIGHT	8	//how many textures are in a columm of the sprite
#define INTERTALE_SPACE	1	//space between two textures

#define SCENE_Xo		BLOCK_SIZE	//how many space there is between the left of the screen and the game
#define SCENE_Yo		BLOCK_SIZE	//how many space there is between the bottom of the screen and the game

#define SCENE_WIDTH		16	//how many blocks wide the scene is
#define SCENE_HEIGHT	11	//how many blocks height the scene is

#define FILENAME		"levels/"	//Format of the files that contains the info of the levels
#define FILENAME_EXT	".txt"

//Image array size
#define NUM_IMG		6
//Image identifiers
#define IMG_BLOCKS	0
#define IMG_PLAYER	1
#define IMG_ENEMY1	2
#define IMG_ENEMY2	3
#define IMG_OBJECTS	4
#define IMG_BOSS	5
#define IMG_MAINMENU 6

#define GAME_WIDTH	(BLOCK_SIZE*SCENE_WIDTH+2*BLOCK_SIZE)
#define GAME_HEIGHT (BLOCK_SIZE*SCENE_HEIGHT+2*BLOCK_SIZE)

#define FPS	60