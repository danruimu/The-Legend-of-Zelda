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
	void Draw(int tex_id, bool mainMenu, char* text);
	void newGameAnimation(int texID);
	int *GetMap();
	char *getId();
	void setId(char Nid[]);
	void setPaused();

private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
	char id[2];
	cEnemy foes[42];
};
