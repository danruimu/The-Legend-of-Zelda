#pragma once

#include "cTexture.h"
#include "cEnemy.h"

class cScene
{
public:
	cScene(void);
	virtual ~cScene(void);

	bool LoadLevel(char* level);
	bool LoadMainMenu(int id);
	bool LoadDungeon(int dungeon);
	void Draw(int tex_id);
	int *GetMap();
	char *getId();
	void setId(char Nid[]);

private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
	char id[2];
	cEnemy foes[42];
};
