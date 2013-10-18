#pragma once
#include "utils.h"
class cObject
{
public:
	cObject(int x,int y,int id);
	~cObject(void);
	void Render(int tex_id);
	bool process();
	cRect getArea();
	bool collides(cRect target);
	void setMovable(int velocity,int direction,int damage);
	int getDamage();
	void setCollectable(int price);
	int getPrice();
	void setAnimated(int *animation,int num_animations,int frame_delay);

private:
	int x,y,id;

	bool collectable;
	int price;

	bool moving;
	int direction,velocity,damage;

	bool animated;
	int *animation;//vector donde guardar los objid animados diferentes
	int frame,seq;
	int frame_delay,num_animations;
};

