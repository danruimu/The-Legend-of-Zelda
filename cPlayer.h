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

struct Sword{
	bool alive;
	int x,y;
	int direction;
	int state;
};

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void Draw(int tex_id,int obj_id);
	bool tirapalante(int *map);
	bool ataca();
	int heal(int num_hearts);//-1 -> full recovery
	int damage(int num_hearts);//devuelve el numero de corazones, quien llame a esto 
	void givePoints(int num_points);
	bool useKey();
	void getKey();
private:
	void printInfo(int obj_id);
	Sword espasa;
	int life;
	int max_life;
	int points;
	int keys;
	int triforce;
	int max_triforces;
};
