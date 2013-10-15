#include "cScene.h"
#include "Globals.h"

cScene::cScene(void)
{
	id[0] = 'H';
	id[1] = '8';
	id[2] = '\0';
	names[LOCKED] = STR_LOCKED;
	names[TRIFORCE] = STR_TRIFORCE;
}

cScene::~cScene(void)
{
}

bool cScene::PrintMainMenu(int id) {
	float xi,yi,xf,yf;
	xi=0.5*(id%2);
	yi=(id<<1)*0.5;
	xf=xi+0.5;
	yf=0.5+yi;
	glBegin(GL_QUADS);
		glTexCoord2f(xi, yi);	glVertex2i(0, GAME_HEIGHT);
		glTexCoord2f(xf, yi);	glVertex2i(GAME_WIDTH, GAME_HEIGHT);
		glTexCoord2f(xf, yf);	glVertex2i(GAME_WIDTH, 0);
		glTexCoord2f(xi, yf);	glVertex2i(0, 0);
	glEnd();

	return true;
}

void cScene::generateCallLevel(){
	float bordeX = (float)INTERTALE_SPACE/(TEXTURES_WIDTH*(TILE_SIZE+INTERTALE_SPACE));
	float bordeY = (float)INTERTALE_SPACE/(TEXTURES_HEIGHT*(TILE_SIZE+INTERTALE_SPACE));
	float blockX = (float)TILE_SIZE/(TEXTURES_WIDTH*(TILE_SIZE+INTERTALE_SPACE));
	float blockY = (float)TILE_SIZE/(TEXTURES_HEIGHT*(TILE_SIZE+INTERTALE_SPACE));

	id_DL=glGenLists(1);
	glNewList(id_DL,GL_COMPILE);
		glBegin(GL_QUADS);
			int x,y,px,py,pos,i,j;
			float coordx_tile, coordy_tile;
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px = SCENE_Xo;
				py = SCENE_Yo+BLOCK_SIZE*j;
				for(i=0;i<SCENE_WIDTH;i++){
					pos = map[(j*SCENE_WIDTH)+i];
					x = pos%TEXTURES_WIDTH;
					y = pos/TEXTURES_WIDTH;
					coordx_tile = x*(blockX+bordeX);
					coordy_tile = y*(blockY+bordeY);

					glTexCoord2f(coordx_tile,coordy_tile+blockY		  );	glVertex2i(px           ,py           );
					glTexCoord2f(coordx_tile+blockX,coordy_tile+blockY);	glVertex2i(px+BLOCK_SIZE,py           );
					glTexCoord2f(coordx_tile+blockX,coordy_tile       );	glVertex2i(px+BLOCK_SIZE,py+BLOCK_SIZE);
					glTexCoord2f(coordx_tile       ,coordy_tile       );	glVertex2i(px           ,py+BLOCK_SIZE);
					px+=BLOCK_SIZE;
				}
			}
		glEnd();
	glEndList();
}

bool cScene::LoadLevel(char level[])
{
	FILE *fd;
	char buffer[42];
	char coma;
	int i,j,tile,n;

	sprintf(buffer,"%s%s%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

	fd=fopen(buffer,"r");
	if(fd==NULL) return false;
	for(j=SCENE_HEIGHT-1;j>=0;j--){
		for(i=0;i<SCENE_WIDTH;i++){
			fscanf(fd,"%d",&tile);
			tile--;
			map[(j*SCENE_WIDTH)+i] = tile;
			fscanf(fd,"%c",&coma);//pass coma
		}
		fscanf(fd,"%c",&coma); //pass enter
	}
	fscanf(fd,"%d",&n);
	fscanf(fd,"%c",&coma);//pass enter
	for(i=0;i<n;i++){
		fscanf(fd,"%s",buffer);
		prop[i] = strcmp(names[i],buffer) == 0;
	}
	fclose(fd);
	setId(level);
	generateCallLevel();
	return true;
}

void cScene::setId(char Nid[]){
	id[0] = Nid[0];
	id[1] = Nid[1];
}

void printText( int x, int y, char *st, void *font ){
	int i,len;
	len = strlen(st);
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2i( x, y);
	for( i=0; i < len; i++){
		glutBitmapCharacter(font, st[i]); // Print a character on the screen
	}
	glColor3f(1.0, 1.0, 1.0);
}

void cScene::Draw(int tex_id, bool mainMenu, char* text[], int currentText,int state)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	if(mainMenu) {
		PrintMainMenu(state);
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*1.5, SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE, text[currentText], GLUT_BITMAP_HELVETICA_18);
		currentText = (currentText + 1)%3;
		printText(SCENE_WIDTH*BLOCK_SIZE/2+BLOCK_SIZE*3, SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE*2, text[currentText], GLUT_BITMAP_HELVETICA_12);
		currentText = (currentText + 1)%3;
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*3, SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE*2, text[currentText], GLUT_BITMAP_HELVETICA_12);
	}
	else{
		glCallList(id_DL);
	}
	glDisable(GL_TEXTURE_2D);
}

void cScene::newGameAnimation(int texID,int currentAnimation) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);

	glColor3f(1.0, 0.0, 0.0);
	PrintMainMenu(currentAnimation);
	glutSwapBuffers();
	Sleep(200);

	glColor3f(0.0, 1.0, 0.0);
	PrintMainMenu(currentAnimation);
	glutSwapBuffers();
	Sleep(200);

	glColor3f(0.0, 0.0, 1.0);
	PrintMainMenu(currentAnimation);
	glutSwapBuffers();
	Sleep(200);
	
	glColor3f(1.0, 1.0, 1.0);
	PrintMainMenu(currentAnimation);
	glutSwapBuffers();
	Sleep(200);
	glDisable(GL_TEXTURE_2D);
	//TODO: falta esperarse 2,2 segundos mas

}

int* cScene::GetMap()
{
	return map;
}

char* cScene::getId() {
	return id;
}

void cScene::drawPauseMenu(char *t1, char* t2, int select) {
	printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE/2, SCENE_HEIGHT*BLOCK_SIZE/2+BLOCK_SIZE*2, "PAUSE", GLUT_BITMAP_TIMES_ROMAN_24);
	if(select == 0) {
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*3, SCENE_HEIGHT*BLOCK_SIZE/2+BLOCK_SIZE, t1, GLUT_BITMAP_HELVETICA_18);
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*2, SCENE_HEIGHT*BLOCK_SIZE/2, t2, GLUT_BITMAP_HELVETICA_10);
	} else if (select == 1) {
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*2, SCENE_HEIGHT*BLOCK_SIZE/2+BLOCK_SIZE, t1, GLUT_BITMAP_HELVETICA_10);
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*3, SCENE_HEIGHT*BLOCK_SIZE/2, t2, GLUT_BITMAP_HELVETICA_18);
	}
}


int BoxOut(cRect box){
	if(box.bottom <= 0) return DOWN;
	if(box.top >= SCENE_HEIGHT*BLOCK_SIZE) return UP;
	if(box.left <= 0)return LEFT;
	if(box.right >= SCENE_WIDTH*BLOCK_SIZE) return RIGHT;
	return -1;
}

bool isWallkable(int tile) {
	if (tile == 2 || tile == 8 || tile == 14) return true;	//tierra
	if (tile ==  18 || tile == 24 ||tile == 30) return true;	//escaleras verticales
	if (tile >= 126 && tile <= 129 || tile >= 132 && tile <= 135 || tile >= 138 && tile <=141 ) return true;  //esquinas
	if (tile >=75 && tile >= 77 || tile >=81 && tile >= 83 || tile >=87 && tile >= 89) return true;  //desierto
	if (tile >=93 && tile >= 95 || tile >=99 && tile >= 101 || tile >=105 && tile >= 107) return true;  //desierto
	if (tile >=111 && tile >= 113 || tile >=117 && tile >= 119 || tile >=123 && tile >= 125) return true;  //desierto
	if (tile ==  59 || tile == 65 ||tile == 71) return true;	//desierto
	if (tile ==  131 || tile == 137 ||tile == 143) return true;	//puente
	return false;
}

bool isWater(int tile){
	if (tile >=72 && tile >= 74 || tile >=78 && tile >= 81 || tile >=84 && tile >= 86) return true;
	if (tile >=90 && tile >= 92 || tile >=96 && tile >= 98 || tile >=103 && tile >= 105) return true;
	if (tile >=108 && tile >= 110 || tile >=114 && tile >= 116 || tile >=120 && tile >= 122) return true; 
	return false;
}

bool isDoor(int what){
	return (what == 22 || what == 28 || what == 34);
}

bool isLockedDoor(int what){
	return what == 136;
}

int cScene::Process(cRect *BoxOrg){
	cRect Box = *BoxOrg;
	Box.bottom-=SCENE_Yo;
	Box.top-=SCENE_Yo;
	Box.left-=SCENE_Xo;
	Box.right-=SCENE_Xo;
	int out = BoxOut(Box);
	if(out != -1){
		switch (out){
		case UP:
			Box.bottom=0;
			Box.top=BLOCK_SIZE;
			id[1]--;
			break;
		case DOWN:
			Box.top=SCENE_HEIGHT*BLOCK_SIZE;
			Box.bottom=Box.top-BLOCK_SIZE;
			id[1]++;
			break;
		case LEFT:
			Box.right=SCENE_WIDTH*BLOCK_SIZE;
			Box.left=Box.right-BLOCK_SIZE;
			id[0]--;
			break;
		case RIGHT:
			Box.left=0;
			Box.right=BLOCK_SIZE;
			id[0]++;
			break;
		}
		LoadLevel(id);
		BoxOrg->top = Box.top+SCENE_Yo;
		BoxOrg->bottom = Box.bottom+SCENE_Yo;
		BoxOrg->left = Box.left+SCENE_Xo;
		BoxOrg->right = Box.right+SCENE_Xo;
		return OUTLIMITS;
	}
	int numlockeddoors = 0;
	int numdoors = 0;
	int numwalkables = 0;
	int tiles[4];
	tiles[0] = whatsThere(Box.left+1,Box.top-21);
	tiles[1] = whatsThere(Box.right-1,Box.top-21);
	tiles[2] = whatsThere(Box.right-1,Box.bottom+1);
	tiles[3] = whatsThere(Box.left+1,Box.bottom+1);
	for (int i = 0; i < 4; i++){
		if(tiles[i] == LOCKED_DOOR)numlockeddoors++;
		else if(tiles[i] == DOOR)numdoors++;
		else if(tiles[i] == WALKABLE)numwalkables++;
	}
	if(numlockeddoors>=2){
		return COLLIDES_LOCKED_DOOR;
	}
	if(numdoors>=2){
		//TODO fer les mazmorres
		return COLLIDES;
	}
	if(numwalkables == 4){
		return OK;
	}

	return COLLIDES;
}



int cScene::whatsThere(int x,int y){
	int bx,by;
	bx = x/BLOCK_SIZE;
	by = y/BLOCK_SIZE;
	if (isDoor(map[by*SCENE_WIDTH+bx]))return DOOR;
	if (isLockedDoor(map[by*SCENE_WIDTH+bx]))return LOCKED_DOOR;
	if (isWallkable(map[by*SCENE_WIDTH+bx]))return WALKABLE;
	if (isWater(map[by*SCENE_WIDTH+bx]))return WATER;
	return OTHERS;
}

void cScene::unlock(){
	prop[LOCKED] = false;
	for(int i=0;i<SCENE_HEIGHT*SCENE_WIDTH;i++){
		if (isLockedDoor(map[i])){
			map[i] = TILE_DOOR;
		}
	}
	generateCallLevel();
}
