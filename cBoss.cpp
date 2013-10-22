#include "cBoss.h"


cBoss::cBoss(void)
{
	SetWidthHeight(BOSS_WIDTH, BOSS_HEIGHT);
	SetPosition(SCENE_Xo+SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE, SCENE_Yo+SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE*4);
	setAlive(true);
	setMovementDelay(FPS);
}


cBoss::~cBoss(void)
{
}
