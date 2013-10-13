#include "cScene.h"
#include "Globals.h"

cScene::cScene(void)
{
	id[0] = 'H';
	id[1] = '8';
}

cScene::~cScene(void)
{
}

bool cScene::LoadMainMenu(int id) {
	bool res=true;
	float xi,yi,xf,yf;

	switch(id) {
	case 0:
		xi = 0.0; yi = 0.0;
		xf = 0.5; yf = 0.5;
		break;
	case 1:
		xi = 0.5; yi = 0.0;
		xf = 1.0; yf = 0.5;
		break;
	case 2:
		xi = 0.0; yi = 0.5;
		xf = 0.5; yf = 1.0;
		break;
	case 3:
		xi = 0.5; yi = 0.5;
		xf = 1.0; yf = 1.0;
		break;
	}

	id_DL=glGenLists(1);
	glNewList(id_DL,GL_COMPILE);
		glBegin(GL_QUADS);
			glTexCoord2f(xi, yi);	glVertex2i(0, SCENE_Yo*2+SCENE_HEIGHT*BLOCK_SIZE);
			glTexCoord2f(xf, yi);	glVertex2i(SCENE_Xo*2+SCENE_WIDTH*BLOCK_SIZE, SCENE_Yo*2+SCENE_HEIGHT*BLOCK_SIZE);
			glTexCoord2f(xf, yf);	glVertex2i(SCENE_Xo*2+SCENE_WIDTH*BLOCK_SIZE, 0);
			glTexCoord2f(xi, yf);	glVertex2i(0, 0);
		glEnd();
	glEndList();

	return res;
}

bool cScene::LoadLevel(char* level)
{
	bool res;
	FILE *fd;
	char file[16];
	char coma;
	int i,j,px,py,tile;
	float coordx_tile, coordy_tile;
	px=py=0;
	res=true;

	sprintf(file,"%s%s%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

	fd=fopen(file,"r");
	if(fd==NULL) return false;

	id_DL=glGenLists(1);
	glNewList(id_DL,GL_COMPILE);
		glBegin(GL_QUADS);
			float bordeX = (float)INTERTALE_SPACE/(TEXTURES_WIDTH*(TILE_SIZE+INTERTALE_SPACE));
			float bordeY = (float)INTERTALE_SPACE/(TEXTURES_HEIGHT*(TILE_SIZE+INTERTALE_SPACE));
			float blockX = (float)TILE_SIZE/(TEXTURES_WIDTH*(TILE_SIZE+INTERTALE_SPACE));
			float blockY = (float)TILE_SIZE/(TEXTURES_HEIGHT*(TILE_SIZE+INTERTALE_SPACE));
			int x,y;
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px = SCENE_Xo;
				py=SCENE_Yo+BLOCK_SIZE*j;
				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%d",&tile);
					tile--;
						map[(j*SCENE_WIDTH)+i] = tile;
						x = tile%TEXTURES_WIDTH;
						y = tile/TEXTURES_WIDTH;
						coordx_tile = x*(blockX+bordeX);
						coordy_tile = y*(blockY+bordeY);

						glTexCoord2f(coordx_tile,coordy_tile+blockY		  );	glVertex2i(px           ,py           );
						glTexCoord2f(coordx_tile+blockX,coordy_tile+blockY);	glVertex2i(px+BLOCK_SIZE,py           );
						glTexCoord2f(coordx_tile+blockX,coordy_tile       );	glVertex2i(px+BLOCK_SIZE,py+BLOCK_SIZE);
						glTexCoord2f(coordx_tile       ,coordy_tile       );	glVertex2i(px           ,py+BLOCK_SIZE);
						px+=BLOCK_SIZE;
						fscanf(fd,"%c",&coma);
				}
				fscanf(fd,"%c",&coma); //pass enter
			}

		glEnd();
	glEndList();

	fclose(fd);
	setId(level);

	return res;
}

void cScene::setId(char Nid[]){
	id[0] = Nid[0];
	id[1] = Nid[1];
}

void printText( int x, int y, char *st){
	int i,len;
	len = strlen(st);
	//glColor3f(1.,0.,0.);
	glRasterPos2i( x, y);
	for( i=0; i < len; i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
	}
}

void cScene::Draw(int tex_id, bool mainMenu, char* text)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
	if(mainMenu) {
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE, SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE, text);
	}
}

void cScene::newGameAnimation(int texID) {
	glColor3f(1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
	Sleep(100);

	glColor3f(0.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
	Sleep(100);

	glColor3f(0.0, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
	Sleep(100);
	
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

int* cScene::GetMap()
{
	return map;
}

char* cScene::getId() {
	return id;
}