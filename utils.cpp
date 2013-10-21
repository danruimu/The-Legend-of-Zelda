#include "utils.h"

void Draw(int tex_id,int x,int y,int posx,int posy,int OBJECTS_HEIGHT,int OBJECTS_WIDTH){
	float th = 1./OBJECTS_HEIGHT;
	float tw = 1./OBJECTS_WIDTH;
	float tx = ((float)x)/OBJECTS_WIDTH;
	float ty = ((float)y)/OBJECTS_HEIGHT;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(tx,ty+th);	glVertex2i(posx  ,posy);
		glTexCoord2f(tx+tw,ty+th);	glVertex2i(posx+BLOCK_SIZE,posy);
		glTexCoord2f(tx+tw,ty);	glVertex2i(posx+BLOCK_SIZE,posy+BLOCK_SIZE);
		glTexCoord2f(tx,ty);	glVertex2i(posx  ,posy+BLOCK_SIZE);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawObject(int texture_id,int x,int y,int Obj){
	Draw(texture_id,Obj%WIDTH_OBJECT_TEXTURE,Obj/WIDTH_OBJECT_TEXTURE,x,y,HEIGHT_OBJECT_TEXTURE,WIDTH_OBJECT_TEXTURE);
}

void printText( int x, int y, char *st, void *font,float r,float g,float b){
	int i,len;
	len = strlen(st);
	glColor3f(r,g,b);
	glRasterPos2i( x, y);
	for( i=0; i < len; i++){
		glutBitmapCharacter(font, st[i]); // Print a character on the screen
	}
	glColor3f(1.0, 1.0, 1.0);
}


bool folderExists(const char* folderName) {
	DWORD attribs = ::GetFileAttributes(folderName);
	if(attribs == INVALID_FILE_ATTRIBUTES) return false;
	return (attribs & FILE_ATTRIBUTE_DIRECTORY)!= 0;
}

bool collides(cRect a,cRect b){
	if(a.left <= b.left && a.right >= b.left && a.bottom <= b.bottom && a.top >= b.bottom)return true;
	if(a.left <= b.left && a.right >= b.left && a.bottom <= b.top && a.top >= b.top)return true;
	if(a.left <= b.right && a.right >= b.right && a.bottom <= b.bottom && a.top >= b.bottom)return true;
	if(a.left <= b.right && a.right >= b.right && a.bottom <= b.top && a.top >= b.top)return true;
	return false;
}