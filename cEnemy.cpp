#include "cEnemy.h"


cEnemy::cEnemy(void)
{
}

cEnemy::cEnemy(int x, int y, char *type) {
	this->maxLife = 1;
	this->life = maxLife;
	this->x = x;
	this->y = y;
	strcpy(this->type, type);
	this->state = ENEMY_STATE_IDLE;
	this->direction = ENEMY_DOWN;
}

cEnemy::~cEnemy(void)
{
}

void cEnemy::draw() {
	int posx,posy;
	GetPosition(&posx,&posy);
	float bordeX = (0.+ENEMY_SIZE)/LINK_SCENE_TEXTURES_WIDTH;
	float bordeY = (0.+ENEMY_SIZE)/LINK_SCENE_TEXTURES_HEIGHT;
	float blockX = (0.+ENEMY_SIZE)/LINK_SCENE_TEXTURES_WIDTH;
	float blockY = (0.+ENEMY_SIZE)/LINK_SCENE_TEXTURES_HEIGHT;
	int direction = this->direction;
	int state = this->state;
	int xo, yo;
	xo = direction*(blockX+bordeX);
	yo = state*(blockY+bordeY);

	char *file;
	sprintf(file, "%s.png", this->type);

	switch (direction){
	case ENEMY_DOWN:
		DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
		break;
	case DIRECTION_UP:
		yo += (0.+LINK_DESFASE_ATTACK)/LINK_SCENE_TEXTURES_HEIGHT;
		SetPosition(posx,posy+BLOCK_SIZE);
		yo-=blockY;
		DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
		yo+=blockY;
		break;
	case DIRECTION_LEFT:
		xo+= (0.+LINK_DESFASE_ATTACK)/LINK_SCENE_TEXTURES_WIDTH;
		SetPosition(posx-BLOCK_SIZE,posy);
		xo-=blockX;
		DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
		xo+=blockX;
		break;
	case DIRECTION_RIGHT:
		xo-= (0.+LINK_DESFASE_ATTACK)/LINK_SCENE_TEXTURES_WIDTH;
		SetPosition(posx+BLOCK_SIZE,posy);
		xo+=blockX;
		DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
		xo-=blockX;
		break;
	}
	SetPosition(posx,posy);
}