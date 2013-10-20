#pragma once

#include "cBicho.h"
#include "utils.h"
#include "cObject.h"

#define PLAYER_START_CX		8
#define PLAYER_START_CY		5

#define PLAYER_TEXTURE_WIDTH	4
#define PLAYER_TEXTURE_HEIGHT	5
#define LINK_SCENE_TEXTURES_WIDTH		351
#define LINK_SCENE_TEXTURES_HEIGHT	185
#define LINK_DESFASE_ATTACK		6
#define LINK_SIZE				15

#define STATE_LOOKUP		1
#define NUM_TRANS			8

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void Draw(int tex_id,int obj_id);
	bool tirapalante(int *map);
	int ataca();//0 = no ataca; 1 = ataca; 2 = ataca i dispara
	int heal(int num_hearts);//-1 -> full recovery
	int damage(int num_hearts);//devuelve el numero de corazones, quien llame a esto 
	bool fullLife();
	void givePoints(int num_points);
	bool useKey();
	void getKey();
	void Logic(bool pause);
	void sayonaraSword();

	cRect getSwordBox();

	void setGodMode(bool mode);
	bool getGodMode();

private:
	void printInfo(int obj_id);
	int life,max_life;
	int points,keys;
	int triforce,max_triforces;
	cObject *espasa;
	bool godMode;
	int godModeDuration;
};
