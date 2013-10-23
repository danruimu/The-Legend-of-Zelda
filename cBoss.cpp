#include "cBoss.h"

cBoss::cBoss(void)
{
}

cBoss::cBoss(int tex_id)
{
	SetWidthHeight(BOSS_WIDTH, BOSS_HEIGHT);
	SetPosition(SCENE_Xo+SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE, SCENE_Yo+SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE*4);
	setAlive(true);
	setMovementDelay(FPS/2);
	setTex_ID(tex_id);
	SetState(0);
	imBoss = true;
	life = 6;
	damaged = false;
}


cBoss::~cBoss(void)
{
}

cObject* cBoss::dropTriforce(){
	int posx,posy;
	GetPosition(&posx,&posy);
	cObject* triforce = new cObject(posx,posy-BLOCK_SIZE,TRIFORCE_Y);
	int vector[] = {TRIFORCE_Y,TRIFORCE_B};
	triforce->setAnimated(vector,2,FRAME_DELAY);
	triforce->setCollectable(0);
	return triforce;
}

void cBoss::draw() {
	int posx,posy;
	GetPosition(&posx,&posy);
	int movDelay = getMovementDelay();
	if(damaged) {
		--damagedDuration;
		if(damagedDuration > 0 && damagedDuration%2 == 0) {
			glColor3f(1.0, 0.0, 0.0);
		} else {
			damaged = false;
		}
	}
	setMovementDelay(drawBoss(getTex_ID(), movDelay, posx, posy, GetState()));
	if(damaged) {
		//glColor3f(1.0, 1.0, 1.0);
	}
}

int cBoss::process() {
	int movDelay = getMovementDelay();
	int shoot = 0;
	if(movDelay%FPS/2 == 0) {
		if(movDelay == 0) {
			movDelay = FPS;
			shoot = 1;
		}
		setMovementDelay(movDelay);
		int state = GetState();
		state = ++state%5;
		SetState(state);
		int x,y; GetPosition(&x, &y);
		float r = (float)rand()/(float)RAND_MAX;
		if(r>0.5 && (x+BLOCK_SIZE) < SCENE_Xo+BLOCK_SIZE*SCENE_WIDTH-BLOCK_SIZE*2) SetPosition(x+BLOCK_SIZE, y);
		else if((x-BLOCK_SIZE)>SCENE_Xo+BLOCK_SIZE*2 )SetPosition(x-BLOCK_SIZE, y);
	}
	return shoot;
}

void cBoss::setDamaged() {
	damaged = true;
	damagedDuration = FPS;
}

int cBoss::Damage() {
	int life = getLife();
	if(!damaged) {
		life = cEnemy::Damage();
	}
	return life;
}
