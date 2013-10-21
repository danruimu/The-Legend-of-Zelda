#pragma once
#include "Globals.h"

#define String char*

typedef struct {
	int left,top,
		right,bottom;
}cRect;

void Draw(int tex_id,int x,int y,int posx,int posy,int num_objects_row,int num_objects_column);
void DrawObject(int texture_id,int x,int y,int Obj);
void printText( int x, int y, char *st, void *font,float r,float g,float b);
void printChar( int x, int y, char c, void *font,float r,float g,float b);
bool collides(cRect a,cRect b);

bool folderExists(const char* folderName);

