#pragma once

#include "cTexture.h"
#include "cEnemy.h"

#define STR_LOCKED	"locked"
#define LOCKED		0


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
	void setPaused(char *t1, char* t2, int select);

private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
	char id[2];
	bool prop[NUM_PROPS];
	char *names[NUM_PROPS];
	cEnemy foes[42];
};
