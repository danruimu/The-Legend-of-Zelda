#pragma once
#include "utils.h"
#include "cTexture.h"
#include "cEnemy.h"

#define STR_DUNGEON	"dungeon"
#define DUNGEON_PROP		0//if its true indicates that the entry in this level its a dungeon; otherwise it indicates that it's a shop


#define OK						0
#define COLLIDES				1
#define COLLIDES_LOCKED_DOOR	2
#define OUTLIMITS				3
#define DUNGEON					4
#define MARKET					5

#define WALKABLE	0
#define DOOR		1
#define LOCKED_DOOR	2
#define WATER		3
#define OTHERS		-1

#define DOWN	0
#define UP		1
#define RIGHT	2
#define LEFT	3

#define TILE_DOOR	22


class cScene
{
public:
	cScene(void);
	virtual ~cScene(void);

	bool LoadLevel(char* level,bool overridable);
	bool PrintMainMenu(int idMM);
	void Draw(int tex_id, bool mainMenu, char* text[], int currentText,int state);
	void newGameAnimation(int texID,int currentAnimation);
	int *GetMap();
	char *getId();
	void setId(char Nid[]);

	void drawPauseMenu(char *t1, char* t2, char *t3, int select);
	int Process(cRect *Box,String unlockedDoors[]);
	int whatsThere(int x,int y);
	void unlock();


private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
	char id[3];
	bool prop[NUM_PROPS];
	char *names[NUM_PROPS];
	void generateCallLevel();
	int xDoor,yDoor;//indicates the position of the door previusly gone by, if theres one; otherwise this value it's -1
	bool dungeon;//indicates if the current level is a dungeon or a market
	bool LoadDungeon(bool dungeon);
	bool exitingDoor;
};
