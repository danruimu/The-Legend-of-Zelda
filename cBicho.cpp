#include "cBicho.h"
#include "cScene.h"
#include "Globals.h"

cBicho::cBicho(void)
{
	delay=0;
}
cBicho::~cBicho(void){}

cBicho::cBicho(int posx,int posy,int width,int height)
{
	x = posx;
	y = posy;
	w = width;
	h = height;
}
void cBicho::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cBicho::GetPosition(int *posx,int *posy)
{
	*posx = x;
	*posy = y;
}

void cBicho::SetBlock(int tx,int ty)
{
	x = tx * BLOCK_SIZE;
	y = ty * BLOCK_SIZE;
}
void cBicho::GetBlock(int *tx,int *ty)
{
	*tx = x / BLOCK_SIZE;
	*ty = y / BLOCK_SIZE;
}
void cBicho::SetWidthHeight(int width,int height)
{
	w = width;
	h = height;
}
void cBicho::GetWidthHeight(int *width,int *height)
{
	*width = w;
	*height = h;
}

bool cBicho::Collides(cRect *rc)
{
	return ((x>rc->left) && (x+w<rc->right) && (y>rc->bottom) && (y+h<rc->top));
	return false;
}

void cBicho::GetArea(cRect *rc)
{
	rc->left   = x;
	rc->right  = x+w;
	rc->bottom = y;
	rc->top    = y+h;
}

void cBicho::SetArea(cRect rc){
	x = rc.left;
	y = rc.bottom;
}

void cBicho::DrawRect(int tex_id,float xo,float yo,float xf,float yf)
{
	int screen_x,screen_y;
	
	screen_x = x;
	screen_y = y; /*+ (BLOCK_SIZE - TILE_SIZE)*/

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(xo,yo);	glVertex2i(screen_x  ,screen_y);
		glTexCoord2f(xf,yo);	glVertex2i(screen_x+w,screen_y);
		glTexCoord2f(xf,yf);	glVertex2i(screen_x+w,screen_y+h);
		glTexCoord2f(xo,yf);	glVertex2i(screen_x  ,screen_y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cBicho::NextFrame(int init,int max,int frame_delay)
{
	delay++;
	if(delay == frame_delay)
	{
		state = init + ((state-init)+1)%max;
		delay = 0;
	}
}

int cBicho::GetState()
{
	return state;
}
void cBicho::SetState(int s)
{
	state = s;
}

int  cBicho::GetDirection(){
	return direction;
}

void cBicho::SetDirection(int d){
	direction = d;
	delay = 0;
}

int cBicho::GetSpeed(){
	return speed;
}

void cBicho::SetSpeed(int s){
	speed = s;
}

bool cBicho::isAlive() {
	return alive;
}

void cBicho::setAlive(bool alive) {
	this->alive = alive;
}
