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
	void setMovable(bool movable,int velocity,int direction);
private:
	int x,y,id;
	bool collectable,moving,animated;
	int direction,velocity;
	int *animation;//vector donde guardar los objid animados diferentes
	int frame;
	int num_animations;

};

