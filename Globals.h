
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gl/glut.h>

#define TILE_SIZE		16
#define BLOCK_SIZE		32

#define SCENE_TEXTURES_WIDTH	18	//how many textures are in a row of the sprite
#define SCENE_TEXTURES_HEIGHT	8	//how many textures are in a columm of the sprite
#define INTERTALE_SPACE	1	//space between two textures

#define WIDTH_OBJECT_TEXTURE	5
#define HEIGHT_OBJECT_TEXTURE	12

#define NUM_ENEMIES		2

//Id de las texturas de los objectos
#define BLUE_HEART		0
#define HEART_CONTAINER	1
#define KEY				2
#define RUPY			3
#define FAIRY			4

#define RED_HEART		5
#define HALF_HEART		6
#define EMPTY_HEART		7
#define RUPY_X5			8
#define FAIRY_MOV		9

#define TRIFORCE_Y		10
#define BOMB			11
#define MAP_Y			12
#define CROISSANT		13
#define CANDLE_R		14

#define TRIFORCE_B		15
#define FOOD			16
#define MAP_B			17
#define ROCK			18
#define CANDLE_B		19

#define SWORD_DOWN		20
#define SWORD_LEFT		21
#define SWORD_UP		22
#define SWORD_RIGHT		23
#define SHIELD			24

#define A_SWORD_DOWN	25
#define A_SWORD_LEFT	26
#define A_SWORD_UP		27
#define A_SWORD_RIGHT	28
#define ULTIMATE_SWORD	29

#define W_SWORD_DOWN	30
#define W_SWORD_LEFT	31
#define W_WORD_UP		32
#define W_SWORD_RIGHT	33
#define RED_RING		34

#define A_W_SWORD_DOWN	35
#define A_W_SWORD_LEFT	36
#define A_W_WORD_UP		37
#define A_W_SWORD_RIGHT	38
#define BLUE_RING		39

#define ARROW_G_DOWN	40
#define ARROW_G_LEFT	41
#define ARROW_G_UP		42
#define ARROW_G_RIGHT	43
#define COMPASS			44

#define ARROW_R_DOWN	45
#define ARROW_R_LEFT	46
#define ARROW_R_UP		47
#define ARROW_R_RIGHT	48
#define BOW				49

#define ARROW_B_DOWN	50
#define ARROW_B_LEFT	51
#define ARROW_B_UP		52
#define ARROW_B_RIGHT	53
#define BIBLE			54

#define BLUE_RAFT		55
#define RED_RAFT		56
#define GREEN_RAFT		57
#define RED_POTION		58
#define BLUE_POTION		59


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
#define IMG_OBJECTS	2
#define IMG_BOSS	3
#define IMG_MAINMENU 4
#define START_ENEMIES	5
#define IMG_ENEMY_OCTOROK_B	5
#define IMG_ENEMY_FAT_DOG_O	6


#define GAME_WIDTH	BLOCK_SIZE*(SCENE_WIDTH+2)
#define GAME_HEIGHT BLOCK_SIZE*(SCENE_HEIGHT+2)

#define FPS	60
#define NUM_PROPS	4

#define NUM_MAX_GATES 42
#define NUM_MAX_OBJECTS 42
#define	MAX_N_MONSTERS	42

#define DIRECTION_DOWN		0
#define DIRECTION_UP		2
#define DIRECTION_LEFT		1
#define DIRECTION_RIGHT		3

#define OPT_DIR	"opts"
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

//DROPS
#define DROP_HEART	0
#define DROP_RUPPE	1