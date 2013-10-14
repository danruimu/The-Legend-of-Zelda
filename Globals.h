
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

#define OPT_FILE "opts/options.txt"

//Sounds
#define LOZ_ARROW	'a'		//effect for throw arror
#define	LOZ_BOMB_BLOW	'b'	//effect for bomb explosions
#define LOZ_BOMB_DROP	'c'	//effect for bomb drop
#define LOZ_BOOMERANG	'd'	//effect for boomerang shoot
#define LOZ_BOSS_SCREAM_1	'e'	//effect for boss scream 1
#define LOZ_BOSS_SCREAM_2	'f'	//effect for boss scream 2
#define LOZ_CANDLE	'g'		//effect for candle
#define LOZ_DIE	'h'			//effect for Link Die
#define LOZ_FANFARE	'i'		//effect for fanfare
#define LOZ_GET_HEART	'j'	//effect for heart collection
#define LOZ_GET_ITEM	'k'	//effect for item collection
#define LOZ_GET_RUPEE	'l'	//effect for rupee collection
#define LOZ_HIT	'm'		//effect for Link hit to enemy
#define LOZ_HURT	'n'	//effect for Link get hurt
#define LOZ_KEY	'o'		//effect for key collection
#define LOZ_KILL	'p'	//effect when enemy dies
#define LOZ_LOW_HEALTH	'q'	//effect for link low of health (make a loop)
#define LOZ_MAGICAL_ROD	'r'	//effect for magicalrod
#define LOZ_RECORDER	's'	//3 first seconds of the LOZ_MUSIC_WHISTLE
#define LOZ_SECRET	't'		//effect for secret discovery
#define LOZ_SHIELD	'u'		//effect for shield
#define LOZ_STAIRS	'v'		//effect when entering through a door or stairs
#define LOZ_SWORD	'w'		//effect for sword hit
#define LOZ_SWORD_COMBINED	'x'	//effect for sword hit and shoot at the same time
#define LOZ_SWORD_SHOOT	'y'	//effect for sword shoot
#define LOZ_TEXT	'z'		//effect for show text
#define LOZ_UNLOCK	'A'		//effect for door unlocked
#define	LOZ_MUSIC_OVERWORLD	'B'	//music theme for overworld
#define	LOZ_MUSIC_MAIN_MENU	'C'		//music theme for introduction/main menu
#define	LOZ_MUSIC_DEATH_MOUNTAIN	'D'	//music theme for death mountain dungeon
#define LOZ_MUSIC_GAME_OVER	'E'		//music theme for game over (includes LOZ_DIE)
#define LOZ_MUSIC_GET_TRIFORCE	'F'	//music for collection triforce
#define LOZ_MUSIC_UNDERWORLD	'G'	//muic theme for underworld
#define LOZ_MUSIC_WHISTLE	'H'		//music for exit game
