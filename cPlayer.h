#pragma once

#include "cBicho.h"

#define PLAYER_START_CX		8
#define PLAYER_START_CY		5

#define PLAYER_TEXTURE_WIDTH	4;
#define PLAYER_TEXTURE_HEIGHT	5;

#define STATE_LOOKUP		1

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void Draw(int tex_id);
	bool tirapalante(int *map);
};
