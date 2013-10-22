#pragma once
#include "cEnemy.h"
#include "utils.h"

#define BOSS_WIDTH BLOCK_SIZE*2
#define BOSS_HEIGHT BLOCK_SIZE*2

class cBoss :
	public cEnemy
{
public:
	cBoss(void);
	~cBoss(void);

	void draw();
	void process();

private:
	void dropTriforce();
};

