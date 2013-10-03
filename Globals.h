
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gl/glut.h>

#define TILE_SIZE		16
#define BLOCK_SIZE		32
#define TEXTURES_WIDTH	18
#define TEXTURES_HEIGHT	8
#define INTERTALE_SPACE	1
#define SCENE_Xo		TILE_SIZE
#define SCENE_Yo		TILE_SIZE
#define SCENE_WIDTH		16
#define SCENE_HEIGHT	11
#define FILENAME		"level"
#define FILENAME_EXT	".txt"
//Image array size
#define NUM_IMG		5
//Image identifiers
#define IMG_BLOCKS	0
#define IMG_PLAYER	1
#define IMG_ENEMY1	2
#define IMG_ENEMY2	3
#define IMG_SHOOT	4
#define GAME_WIDTH	640
#define GAME_HEIGHT 480