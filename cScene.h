#pragma once

#include "cTexture.h"

class cScene
{
public:
	cScene(void);
	virtual ~cScene(void);

	bool LoadLevel(char* level);
	void Draw(int tex_id);
	int *GetMap();
	char *getId();

private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	//int up[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	//int down[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	//int left[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	//int right[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
	char *id;
};
