#pragma once

#include "cBicho.h"

#define PLAYER_START_CX		8
#define PLAYER_START_CY		5

#define PLAYER_TEXTURE_WIDTH	4
#define PLAYER_TEXTURE_HEIGHT	5
#define LINK_TEXTURES_WIDTH		351
#define LINK_TEXTURES_HEIGHT	185
#define LINK_DESFASE_ATTACK		6
#define LINK_SIZE				15

#define STATE_LOOKUP		1
#define NUM_TRANS			8

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void Draw(int tex_id);
	bool tirapalante(int *map);
};
