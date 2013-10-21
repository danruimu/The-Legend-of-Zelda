#include "cObject.h"


cObject::cObject(int x,int y,int id){
	this->x = x;
	this->y =y;
	this->id = id;
	moving = false;
	animated = false;
	collectable = false;
}


cObject::~cObject(void){
	if(animated)free(animation);
}

void cObject::Render(int tex_id){
	int localId = id;
	if (animated) {
		seq++;
		if(seq==frame_delay){
			seq=0;
			frame++;
			if (frame >= num_animations) frame = 0;
		}
		localId = animation[frame];
	}
	if (moving) localId += direction;
	DrawObject(tex_id,x,y,localId);
	if(collectable && price > 0){
		char buffer[42];
		sprintf(buffer,"%dr",price);
		printText(x,y-BLOCK_SIZE/2 + 2,buffer,GLUT_BITMAP_TIMES_ROMAN_24,1.,1.,1.);
	}
}

bool cObject::process(){
	if(moving){
		x = x + (direction==DIRECTION_RIGHT)*velocity;
		x = x - (direction==DIRECTION_LEFT)*velocity;
		y = y + (direction==DIRECTION_UP)*velocity;
		y = y - (direction==DIRECTION_DOWN)*velocity;
		if (x-SCENE_Xo <= 0 || y-SCENE_Yo<=0 || x >=BLOCK_SIZE*SCENE_WIDTH||y >= BLOCK_SIZE*SCENE_HEIGHT){
			return false;		
		}
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

bool cObject::collides(cRect target){
	if (target.left >= x && target.left <= x+BLOCK_SIZE && target.bottom >= y && target.bottom <= y+BLOCK_SIZE)return true;
	if (target.left >= x && target.left <= x+BLOCK_SIZE && target.top >= y && target.top <= y+BLOCK_SIZE)return true;
	if (target.right >= x && target.right <= x+BLOCK_SIZE && target.bottom >= y && target.bottom <= y+BLOCK_SIZE)return true;
	if (target.right >= x && target.right <= x+BLOCK_SIZE && target.top >= y && target.top <= y+BLOCK_SIZE)return true;
	return false;
}

void cObject::setMovable(int velocity,int direction,int damage){
	moving = true;
	this->velocity = velocity;
	this->direction = direction;
}

int cObject::getDamage(){
	if(moving)return damage;
	else return 0;
}

void cObject::setCollectable(int price){
	collectable = true;
	this->price = price;
}

int cObject::getPrice(){
	if (collectable)return price;
	else return 0;
}

void cObject::setAnimated(int *animation,int num_animations,int frame_delay){
	animated = true;
	frame = 0;
	seq=0;
	this->frame_delay = frame_delay;
	this->num_animations = num_animations;
	this->animation = (int*)malloc(num_animations*sizeof(int));
	for (int i = 0; i < num_animations; i++){
		this->animation[i] = animation[i];
	}
}

bool cObject::isCollectable(){
	return collectable;
}

bool cObject::isMovable(){
	return moving;
}

int cObject::getId(){
	return id;
}
