#pragma once

#include "cTexture.h"
#include "cEnemy.h"

#define STR_LOCKED	"locked"
#define LOCKED		0

#define OK						0
#define COLLIDES				1
#define COLLIDES_LOCKED_DOOR	2
#define OUTLIMITS				3

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

	bool LoadLevel(char* level);
	bool LoadMainMenu(int id);
	bool LoadDungeon(int dungeon);
	void Draw(int tex_id, bool mainMenu, char* text[], int currentText);
	void newGameAnimation(int texID);
	int *GetMap();
	char *getId();
	void setId(char Nid[]);

	void drawPauseMenu(char *t1, char* t2, int select);
	int Process(cRect *Box);
	int whatsThere(int x,int y);
	void unlock();


private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
	char id[3];
	bool prop[NUM_PROPS];
	char *names[NUM_PROPS];
	cEnemy foes[42];
	void generateCallLevel();
};
