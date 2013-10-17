#include "cObject.h"


cObject::cObject(int x,int y,int id){
	this->x = x;
	this->y =y;
	this->id = id;
	this->moving = false;
	this->animated = false;
	this->collectable = false;
}


cObject::~cObject(void){
}

void cObject::Render(int tex_id){
	int localId = id;
	if (animated) localId = animation[frame];
	if (moving) localId += direction;
	DrawObject(tex_id,x,y,localId);
}

bool cObject::process(){
	if(moving){
		x = x + (direction==DIRECTION_RIGHT)*velocity;
		x = x - (direction==DIRECTION_LEFT)*velocity;
		y = y + (direction==DIRECTION_UP)*velocity;
		y = y - (direction==DIRECTION_DOWN)*velocity;
	}
	if (x-SCENE_Xo <= 0 || y-SCENE_Yo<=0 || x >=BLOCK_SIZE*SCENE_WIDTH||y >= BLOCK_SIZE*SCENE_HEIGHT){
		return false;		
	}
	if(animated){
		frame += 1;
		if (frame >= num_animations) frame = 0;
	}
	return true;
}

cRect cObject::getArea(){
	cRect rect;
	rect.bottom = y;
	rect.top = y+BLOCK_SIZE;
	rect.left = x;
	rect.right = x+BLOCK_SIZE;
	return rect;
}

void cObject::setMovable(bool movable,int velocity,int direction){
	moving = movable;
	this->velocity = velocity;
	this->direction = direction;
}
