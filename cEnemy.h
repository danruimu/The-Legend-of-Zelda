#pragma once
#include "cbicho.h"
#define	ENEMY_DOWN	0
#define	ENEMY_RIGHT	1
#define	ENEMY_UP	2
#define	ENEMY_LEFT	3

#define	ENEMY_STATE_IDLE	0
#define	ENEMY_STATE_MOVE	1

#define ENEMY_SIZE	16

class cEnemy :
	public cBicho
{
public:
	cEnemy(void);
	cEnemy(int x, int y, char *type);
	~cEnemy(void);

	void draw();

private:
	int life;
	int maxLife;
	char *type;
	int x;
	int y;
	int state;
	int direction;
};

