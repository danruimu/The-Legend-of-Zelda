
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
#define NUM_IMG		7
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
#define NUM_PROPS	4

//Sounds
#define LOZ_ARROW	'a'
#define	LOZ_BOMB_BLOW	'b'
#define LOZ_BOMB_DROP	'c'
#define LOZ_BOOMERANG	'd'
#define LOZ_BOOS_SCREAM_1	'e'
#define LOZ_BOOS_SCREAM_2	'f'
#define LOZ_CANDLE	'g'
#define LOZ_DIE	'h'
#define LOZ_FANFARE	'i'
#define LOZ_GET_HEART	'j'
#define LOZ_GET_ITEM	'k'
#define LOZ_GET_RUPEE	'l'
#define LOZ_HIT	'm'
#define LOZ_HURT	'n'
#define LOZ_KEY	'o'
#define LOZ_KILL	'p'
#define LOZ_LOW_HEALTH	'q'
#define LOZ_MAGICAL_ROD	'r'
#define LOZ_RECORDER	's'
#define LOZ_SECRET	't'
#define LOZ_SHIELD	'u'
#define LOZ_STAIRS	'v'
#define LOZ_SWORD	'w'
#define LOZ_SWORD_COMBINED	'x'
#define LOZ_SWORD_SHOOT	'y'
#define LOZ_TEXT	'z'
#define LOZ_UNLOCK	'A'
#define	LOZ_MUSIC_OVERWORLD	'B'
#define	LOZ_MUSIC_MAIN_MENU	'C'
#define	LOZ_MUSIC_DEATH_MOUNTAIN	'D'
#define LOZ_MUSIC_GAME_OVER	'E'
#define LOZ_MUSIC_GET_TRIFORCE	'F'
#define LOZ_MUSIC_UNDERWORLD	'G'
#define LOZ_MUSIC_WHISTLE	'H'