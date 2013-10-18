#pragma once

#include "cTexture.h"
#include "utils.h"

#define FRAME_DELAY		4
#define FRAME_DELAY_WALK	10
#define STEP_LENGTH		4

#define STATE_IDLE			1
#define STATE_MOVE			0
#define STATE_ATTACK_1		2
#define STATE_ATTACK_2		3
#define STATE_SWORD			4


class cBicho
{
public:
	cBicho(void);
	cBicho(int x,int y,int w,int h);
	~cBicho(void);

	void SetPosition(int x,int y);
	void GetPosition(int *x,int *y);
	void SetBlock(int tx,int ty);
	void GetBlock(int *tx,int *ty);

	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int *w,int *h);

	bool Collides(cRect *rc);
	void GetArea(cRect *rc);
	void SetArea(cRect rc);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);


	int  GetState();
	void SetState(int s);

	int  GetDirection();
	void SetDirection(int s);

	void NextFrame(int init,int max,int frame_delay);

	int GetSpeed();
	void SetSpeed(int s);

	bool isAlive();
	void setAlive(bool alive);
	
private:
	int x,y;
	int w,h;
	int direction;
	int state;
	int speed;

	int delay;
	bool alive;
};
