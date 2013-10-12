#pragma once
#include "cbicho.h"
#define	ENEMY_DOWN	0
#define	ENEMY_RIGHT	1
#define	ENEMY_UP	2
#define	ENEMY_LEFT	3

#define	ENEMY_STATE_IDLE	0
#define	ENEMY_STATE_MOVE	1
class cEnemy :
	public cBicho
{
public:
	cEnemy(void);
	~cEnemy(void);
};

