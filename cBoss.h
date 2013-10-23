#pragma once
#include "cEnemy.h"
#include "utils.h"
#include "cObject.h"

#define BOSS_WIDTH BLOCK_SIZE*2
#define BOSS_HEIGHT BLOCK_SIZE*2

class cBoss :
	public cEnemy
{
public:
	cBoss(void);
	cBoss(int tex_id);
	~cBoss(void);

	void draw();
	int process();
	cObject* dropTriforce();
	void setDamaged();
	int Damage();
private:
	int damagedDuration;
	bool damaged;
};

