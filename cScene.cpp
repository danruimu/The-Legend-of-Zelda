#include "cScene.h"
#include <algorithm>

int BoxOut(cRect box){
	if(box.bottom < SCENE_Yo) return DOWN;
	if(box.top > SCENE_HEIGHT*BLOCK_SIZE + SCENE_Yo) return UP;
	if(box.left < SCENE_Xo)return LEFT;
	if(box.right > SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE) return RIGHT;
	return -1;
}

bool isWater(int tile){
	if (tile >=72 && tile <= 74 || tile >=78 && tile <= 81 || tile >=84 && tile <= 86) return true;
	if (tile >=90 && tile <= 92 || tile >=96 && tile <= 98 || tile >=103 && tile <= 105) return true;
	if (tile >=108 && tile <= 110 || tile >=114 && tile <= 116 || tile >=120 && tile <= 122) return true; 
	return false;
}

bool isWallkable(int tile) {
	if (isWater(tile))return false;//caso de que link consiga la barca cambiar esto
	if (tile == 2 || tile == 8 || tile == 14) return true;	//tierra
	if (tile ==  18 || tile == 24 ||tile == 30) return true;	//escaleras verticales
	if (tile >= 126 && tile <= 129 || tile >= 132 && tile <= 135 || tile >= 138 && tile <=141 ) return true;  //esquinas
	if (tile >=75 && tile <= 77 || tile >=81 && tile <= 83 || tile >=87 && tile <= 89) return true;  //desierto
	if (tile >=93 && tile <= 95 || tile >=99 && tile <= 101 || tile >=105 && tile <= 107) return true;  //desierto
	if (tile >=111 && tile <= 113 || tile >=117 && tile <= 119 || tile >=123 && tile <= 125) return true;  //desierto
	if (tile ==  58 || tile == 64 ||tile == 70) return true;	//desierto
	if (tile ==  131 || tile == 137 ||tile == 143) return true;	//puente
	return false;
}

void zSort(int v[],int d[]){
	int n = 3;
	for(int i = 0; i<4; ++i) {
		n = 3;
		while(n>0) {
			if(v[n] > v[n-1]) {
				int aux = v[n], auxd = d[n];
				v[n-1] = v[n]; d[n-1] = d[n];
				v[n] = aux; d[n] = aux;
			}
			--n;
		}
	}
}

bool isDoor(int what){
	return (what == 22 || what == 28 || what == 34);
}

bool isLockedDoor(int what){
	return what == 136;
}

bool isStairs(int what){
	return (what == 0 || what == 6 || what == 12);
}

int cScene::findTextureId(String str, cData *data){
	for (int i = 0; i < NUM_ENEMIES; i++){
		if(strcmp(str, enemyTypes[i]) == 0) return data->GetID(START_ENEMIES+i);
	}
	return -1;
}

cScene::cScene(void)
{
	id[0] = 'H';
	id[1] = '8';
	id[2] = '\0';
	names[DUNGEON_PROP] = STR_DUNGEON;
	dungeon = false;
	exitingDoor = false;

	nEnemies = 0;
	nObjects = 0;

	//Enemy Types;
	enemyTypes[0] = OCTOROK_B;
	enemyTypes[1] = FAT_DOG_O;
}

cScene::~cScene(void){
	freeEnemies();
	freeObjects();
}

bool cScene::isMarket(){
	return dungeon&&!prop[DUNGEON_PROP];
}

bool cScene::PrintMainMenu(int idMM) {
	float xi,yi,xf,yf;
	xi=0.5*(idMM%2);
	yi=(idMM<<1)*0.5;
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
	float bordeX = (float)INTERTALE_SPACE/(SCENE_TEXTURES_WIDTH*(TILE_SIZE+INTERTALE_SPACE));
	float bordeY = (float)INTERTALE_SPACE/(SCENE_TEXTURES_HEIGHT*(TILE_SIZE+INTERTALE_SPACE));
	float blockX = (float)TILE_SIZE/(SCENE_TEXTURES_WIDTH*(TILE_SIZE+INTERTALE_SPACE));
	float blockY = (float)TILE_SIZE/(SCENE_TEXTURES_HEIGHT*(TILE_SIZE+INTERTALE_SPACE));

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
					x = pos%SCENE_TEXTURES_WIDTH;
					y = pos/SCENE_TEXTURES_WIDTH;
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

bool cScene::LoadLevel(char level[],bool overrided, cData *data)
{
	FILE *fd;
	char buffer[42];
	char coma;
	int i,j,tile,n;
	i=0;
	bossAlive = false;
	while (i<nObjects) {
		if(objects[i]!=nullptr) {
			free(objects[i]);
			objects[i] = nullptr;
		}
		i++;
	}
	nObjects=0;
	i=0;
	while (i<nEnemies) {
		if(enemies[i]!=nullptr) {
			free(enemies[i]);
			enemies[i] = nullptr;
		}
		i++;
	}
	nEnemies = 0;
	//buidar monstres
	sprintf(buffer,"%s%s%s",(char *)FILENAME,level,(char *)FILENAME_EXT);
	if (!dungeon)xDoor = yDoor = -1;
	fd=fopen(buffer,"r");
	if(fd==NULL) return false;
	for(j=SCENE_HEIGHT-1;j>=0;j--){
		for(i=0;i<SCENE_WIDTH;i++){
			fscanf(fd,"%d",&tile);
			tile--;
			if(!dungeon && (isLockedDoor(tile) || isDoor(tile))){
				if(overrided) tile = TILE_DOOR;
				yDoor = j;
				xDoor = i;
			}
			map[(j*SCENE_WIDTH)+i] = tile;
			fscanf(fd,"%c",&coma);//pass coma
		}
		fscanf(fd,"%c",&coma); //pass enter
	}
	if (!dungeon){// de momento las dungeons no tienen propiedades
		fscanf(fd,"%d",&n);//leemos el numero de propiedades de la escena
		fscanf(fd,"%c",&coma);//pass enter
		for(i=0;i<n;i++){//leemos las propiedades per se
			fscanf(fd,"%s",buffer);
			prop[i] = strcmp(names[i],buffer) == 0;
		}
		setId(level);//seteamos el nivel
	}
	
	//Read Enemies from level
	int nEnem;
	fscanf(fd, "%d", &nEnem);
	if ( nEnem > 0) {
		while (nEnem > 0) {
			char *enemyType = (char*) malloc(42);
			int quants = 0;
			fscanf(fd, "%s%d", enemyType, &quants);
			int posx, posy;
			int tex_id = findTextureId(enemyType, data);
			for(int i=0; i<quants; ++i) {
				fscanf(fd, "%d%d", &posx, &posy);
				if(tex_id>=0){
					enemies[nEnemies+i] = new cEnemy(SCENE_Xo+posx*BLOCK_SIZE,SCENE_Yo+ posy*BLOCK_SIZE, enemyType, tex_id,FPS/12);
					enemies[nEnemies+i]->SetSpeed(BLOCK_SIZE/8);
					enemies[nEnemies+i]->setIA(RAND);
				}
			}
			nEnem -= quants;
			nEnemies += quants;
		}
	}

	fclose(fd);
	generateCallLevel();

	return true;
}

char* cScene::getxDooryDoor(){
	char* buffer = (char*) malloc(42);
	sprintf(buffer,"%d %d",xDoor,yDoor);
	return buffer;
}

void cScene::setxDooryDoor(int xDoor,int yDoor){
	this->xDoor=xDoor;
	this->yDoor=yDoor;
}

void cScene::setId(char Nid[]){
	id[0] = Nid[0];
	id[1] = Nid[1];
	id[2] = '\0';
}

void cScene::Draw(int tex_id, int obj_id,bool mainMenu, char* text[], int currentText,int state)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	if(mainMenu) {
		dungeon=false;
		exitingDoor = false;
		bossAlive=false;
		PrintMainMenu(state);
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*1.5, SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE, text[currentText], GLUT_BITMAP_HELVETICA_18,0,0,0);
		currentText = (currentText + 1)%3;
		printText(SCENE_WIDTH*BLOCK_SIZE/2+BLOCK_SIZE*3, SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE*2, text[currentText], GLUT_BITMAP_HELVETICA_12,0,0,0);
		currentText = (currentText + 1)%3;
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*3, SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE*2, text[currentText], GLUT_BITMAP_HELVETICA_12,0,0,0);
	}
	else{
		glCallList(id_DL);
		int i=0;
		while(i<nObjects){
			if(objects[i]!=nullptr)
				objects[i]->Render(obj_id);
			i++;
		}

		if(dungeon && !prop[DUNGEON_PROP])printText(BLOCK_SIZE,0,"F5-->Save the game      F4-->Load last saved game",GLUT_BITMAP_TIMES_ROMAN_24,1,1,1);

		this->drawEnemies();
		if(bossAlive) {
			boss.draw();
		}

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

	for(float i = 1.0; i>=0; i-=0.045) {
		glColor3f(i, i, i);
		PrintMainMenu(currentAnimation);
		glutSwapBuffers();
		Sleep(100);
	}
	glDisable(GL_TEXTURE_2D);
}

int* cScene::GetMap()
{
	return map;
}

char* cScene::getId() {
	return id;
}

void cScene::drawPauseMenu(char *t1, char* t2, char *t3, int select) {
	printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE/2, SCENE_HEIGHT*BLOCK_SIZE/2+BLOCK_SIZE*2, "PAUSE", GLUT_BITMAP_TIMES_ROMAN_24,0,0,0);
	if(select == 0) {
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*3, SCENE_HEIGHT*BLOCK_SIZE/2+BLOCK_SIZE, t1, GLUT_BITMAP_HELVETICA_18,0,0,0);
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*2, SCENE_HEIGHT*BLOCK_SIZE/2, t2, GLUT_BITMAP_HELVETICA_10,0,0,0);
		printText(SCENE_WIDTH*BLOCK_SIZE/2+BLOCK_SIZE/2, SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE, t3, GLUT_BITMAP_HELVETICA_10,0,0,0);
	} else if (select == 1) {
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*2, SCENE_HEIGHT*BLOCK_SIZE/2+BLOCK_SIZE, t1, GLUT_BITMAP_HELVETICA_10,0,0,0);
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*3, SCENE_HEIGHT*BLOCK_SIZE/2, t2, GLUT_BITMAP_HELVETICA_18,0,0,0);
		printText(SCENE_WIDTH*BLOCK_SIZE/2+BLOCK_SIZE/2, SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE, t3, GLUT_BITMAP_HELVETICA_10,0,0,0);
	} else if (select == 2) {
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*2, SCENE_HEIGHT*BLOCK_SIZE/2+BLOCK_SIZE, t1, GLUT_BITMAP_HELVETICA_10,0,0,0);
		printText(SCENE_WIDTH*BLOCK_SIZE/2-BLOCK_SIZE*2, SCENE_HEIGHT*BLOCK_SIZE/2, t2, GLUT_BITMAP_HELVETICA_10,0,0,0);
		printText(SCENE_WIDTH*BLOCK_SIZE/2+BLOCK_SIZE/8, SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE, t3, GLUT_BITMAP_HELVETICA_18,0,0,0);
	}
}

int cScene::Process(cRect *BoxOrg,String unlockedDoors[],String triforcesCollected[], cData *data, cPlayer *link){
	cRect Box = *BoxOrg;
	char *oldId = (char*) malloc(3);
	oldId = getId();

	if(bossAlive) {
		int stateBoss = boss.process();
		if(stateBoss == 1) {    //SHOOT
			int xBoss,yBoss; boss.GetPosition(&xBoss,&yBoss);
			objects[nObjects] = new cObject(xBoss,yBoss-BLOCK_SIZE,ARROW_B_DOWN);
			objects[nObjects]->setMovable(STEP_LENGTH*2,DIRECTION_DOWN,1);
			++nObjects;
		}
	}

	int i=0;
	while (i<nEnemies){
		if(enemies[i]!=nullptr){
			cRect box;
			int sx,sy;
			int speedEnem = enemies[i]->GetSpeed();
			int directionOld = enemies[i]->GetDirection();
			int direction = enemies[i]->SetNewDirection();
			enemies[i]->GetArea(&box);

			int tilesEnem[] = {-1, -1, -1, -1};
			sx=box.left;
			sy=box.bottom;
			switch(direction) {
			case ENEMY_DOWN:
				box.top-=speedEnem;
				box.bottom-=speedEnem;
				sy-=BLOCK_SIZE;
				break;
			case ENEMY_LEFT:
				box.right-=speedEnem;
				box.left-=speedEnem;
				sx-=BLOCK_SIZE;
				break;
			case ENEMY_UP:
				box.top+=speedEnem;
				box.bottom+=speedEnem;
				sy+=BLOCK_SIZE;
				break;
			case ENEMY_RIGHT:
				box.right+=speedEnem;
				box.left+=speedEnem;
				sx+=BLOCK_SIZE;
				break;
			}
			if (BoxOut(box) == -1){
				tilesEnem[0] = whatsThereMonsterVersion(box.left+1,box.top-1);
				tilesEnem[1] = whatsThereMonsterVersion(box.right-1,box.top-1);
				tilesEnem[2] = whatsThereMonsterVersion(box.right-1,box.bottom+1);
				tilesEnem[3] = whatsThereMonsterVersion(box.left+1,box.bottom+1);
				if(tilesEnem[0] == WALKABLE && tilesEnem[1] == WALKABLE && tilesEnem[2] == WALKABLE && tilesEnem[3] == WALKABLE) {
					enemies[i]->process();
				}
				if(directionOld!=direction){
					int proy = enemies[i]->shoot();
					if (proy!= 0){
						objects[nObjects] = new cObject(sx,sy,proy);
						objects[nObjects]->setMovable(speedEnem,direction,1);
						nObjects++;
					}
				}
			}
		}
		i++;
	}

	int out = BoxOut(Box);
	if(out != -1){
		bool overridable = false;
		if(dungeon){
			dungeon = false;
			Box.left = SCENE_Xo + xDoor*BLOCK_SIZE;
			Box.right = Box.left + BLOCK_SIZE;
			Box.bottom = SCENE_Yo + yDoor*BLOCK_SIZE;
			Box.top =  Box.bottom+BLOCK_SIZE;
			overridable = true;
		}
		else{
			switch (out){
			case UP:
				Box.bottom=SCENE_Yo;
				Box.top=Box.bottom + BLOCK_SIZE;
				id[1]--;
				break;
			case DOWN:
				Box.top=SCENE_HEIGHT*BLOCK_SIZE + SCENE_Yo;
				Box.bottom=Box.top-BLOCK_SIZE;
				id[1]++;
				break;
			case LEFT:
				Box.right=SCENE_WIDTH*BLOCK_SIZE + SCENE_Xo;
				Box.left=Box.right-BLOCK_SIZE;
				id[0]--;
				break;
			case RIGHT:
				Box.left=SCENE_Xo;
				Box.right=Box.left - BLOCK_SIZE;
				id[0]++;
				break;
			}
		
			int i = 0;
			while(unlockedDoors[i]!=nullptr && !overridable){
				int comp = strcmp(unlockedDoors[i],id);
				if (comp == 0)overridable = true;
				i++;
			}
		}

		//LoadLevel(id,overridable, data);
		if(!exitingDoor) {
			LoadLevelAnimation(oldId, id, out,data->GetID(IMG_OBJECTS),data->GetID(IMG_BLOCKS), link, data->GetID(IMG_PLAYER));
		} else bossAlive = false;

		LoadLevel(id,overridable, data);
		BoxOrg->top = Box.top;
		BoxOrg->bottom = Box.bottom;
		BoxOrg->left = Box.left;
		BoxOrg->right = Box.right;
		return OUTLIMITS;
	}
	int numlockeddoors = 0;
	int numdoors = 0;
	int numwalkables = 0;
	int tiles[4];
	tiles[0] = whatsThere(Box.left+10,Box.top-21);
	tiles[1] = whatsThere(Box.right-10,Box.top-21);
	tiles[2] = whatsThere(Box.right-10,Box.bottom+1);
	tiles[3] = whatsThere(Box.left+10,Box.bottom+1);
	if (tiles[0] == HURT || tiles[1] == HURT || tiles[2] == HURT || tiles[3] == HURT) {
		BoxOrg->bottom +=2; BoxOrg->left +=2; BoxOrg->top -=2; BoxOrg->right -=2;

		bool pos[] = { false, false, false, false, false, false, false, false}; //DOWN, UP, RIGHT, LEFT, DOWN-LEFT, UP-LEFT, UP-RIGHT, DOWN-RIGHT
		pos[DOWN] = (whatsThere(BoxOrg->right, BoxOrg->bottom-BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->left, BoxOrg->bottom-BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->right, BoxOrg->top-BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->left, BoxOrg->top-BLOCK_SIZE) == WALKABLE);

		pos[UP] = (whatsThere(BoxOrg->right, BoxOrg->bottom+BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->left, BoxOrg->bottom+BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->right, BoxOrg->top+BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->left, BoxOrg->top+BLOCK_SIZE) == WALKABLE);

		pos[RIGHT] = (whatsThere(BoxOrg->right+BLOCK_SIZE, BoxOrg->bottom) == WALKABLE && whatsThere(BoxOrg->left+BLOCK_SIZE, BoxOrg->bottom) == WALKABLE && whatsThere(BoxOrg->right+BLOCK_SIZE, BoxOrg->top) == WALKABLE && whatsThere(BoxOrg->left+BLOCK_SIZE, BoxOrg->top) == WALKABLE);

		pos[LEFT] = (whatsThere(BoxOrg->right-BLOCK_SIZE, BoxOrg->bottom) == WALKABLE && whatsThere(BoxOrg->left-BLOCK_SIZE, BoxOrg->bottom) == WALKABLE && whatsThere(BoxOrg->right-BLOCK_SIZE, BoxOrg->top) == WALKABLE && whatsThere(BoxOrg->left-BLOCK_SIZE, BoxOrg->top) == WALKABLE);

		pos[DOWN_LEFT] = (whatsThere(BoxOrg->left-BLOCK_SIZE, BoxOrg->top-BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->right-BLOCK_SIZE, BoxOrg->top-BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->left-BLOCK_SIZE, BoxOrg->bottom-BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->right-BLOCK_SIZE, BoxOrg->bottom-BLOCK_SIZE) == WALKABLE);

		pos[UP_LEFT] = (whatsThere(BoxOrg->left-BLOCK_SIZE, BoxOrg->top+BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->right-BLOCK_SIZE, BoxOrg->top+BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->left-BLOCK_SIZE, BoxOrg->bottom+BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->right-BLOCK_SIZE, BoxOrg->bottom+BLOCK_SIZE) == WALKABLE);

		pos[UP_RIGHT] = (whatsThere(BoxOrg->left+BLOCK_SIZE, BoxOrg->top+BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->right+BLOCK_SIZE, BoxOrg->top+BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->left+BLOCK_SIZE, BoxOrg->bottom+BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->right+BLOCK_SIZE, BoxOrg->bottom+BLOCK_SIZE) == WALKABLE);

		pos[DOWN_RIGHT] = (whatsThere(BoxOrg->left+BLOCK_SIZE, BoxOrg->top-BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->right+BLOCK_SIZE, BoxOrg->top-BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->left+BLOCK_SIZE, BoxOrg->bottom-BLOCK_SIZE) == WALKABLE && whatsThere(BoxOrg->right+BLOCK_SIZE, BoxOrg->bottom-BLOCK_SIZE) == WALKABLE);

		BoxOrg->bottom -=2; BoxOrg->left -=2; BoxOrg->top +=2; BoxOrg->right +=2;

		if(tiles[0] == HURT && tiles[1] == HURT) {
			if(pos[DOWN]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
			} else if(pos[DOWN_LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			} else if(pos[DOWN_RIGHT]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			} else if(pos[UP_LEFT]) {
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
			} else if(pos[UP_RIGHT]) {
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			} else if(pos[UP]) {
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			}
		} else if(tiles[1] == HURT && tiles[2] == HURT) {
			if(pos[LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
			} else if(pos[UP]) {
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[DOWN]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[UP_LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[DOWN_LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[UP_RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[DOWN_RIGHT]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			} else if(pos[RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			}
		} else if(tiles[2] == HURT && tiles[3] == HURT) {
			if(pos[UP]) {
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
			} else if(pos[RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			} else if(pos[UP_LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[UP_RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[DOWN_LEFT]){
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[DOWN_RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[DOWN]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			}
		} else if(tiles[3] == HURT && tiles[0] == HURT) {
			if(pos[RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			} else if(pos[UP]) {
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[DOWN]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[UP_RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[DOWN_RIGHT]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			} else if(pos[UP_LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[DOWN_LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
			}
		} else if(tiles[0] == HURT) {
			if(pos[DOWN_RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[DOWN]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			} else if(pos[DOWN_LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[UP_RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
			} else if(pos[UP]) {
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[UP_LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			}
		} else if(tiles[1] == HURT) {
			if(pos[DOWN_LEFT]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
			} else if(pos[DOWN]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
			} else if(pos[DOWN_RIGHT]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			} else if(pos[UP_LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			} else if(pos[UP]) {
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[UP_RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			}
		} else if(tiles[2] == HURT) {
			if(pos[UP_LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[UP]) {
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
			} else if(pos[UP_RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[DOWN_LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			} else if(pos[DOWN]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[DOWN_RIGHT]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			}
		} else if(tiles[3] == HURT) {
			if(pos[UP_RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[UP]) {
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[RIGHT]) {
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			} else if(pos[UP_LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom += BLOCK_SIZE; BoxOrg->top += BLOCK_SIZE;
			} else if(pos[DOWN_RIGHT]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
				BoxOrg->left += BLOCK_SIZE; BoxOrg->right += BLOCK_SIZE;
			} else if(pos[LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
			} else if(pos[DOWN]) {
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			} else if(pos[DOWN_LEFT]) {
				BoxOrg->left -= BLOCK_SIZE; BoxOrg->right -= BLOCK_SIZE;
				BoxOrg->bottom -= BLOCK_SIZE; BoxOrg->top -= BLOCK_SIZE;
			}
		}
		if(BoxOrg->bottom <= SCENE_Yo)  { BoxOrg->bottom = SCENE_Yo + BLOCK_SIZE; BoxOrg->top = SCENE_Yo + BLOCK_SIZE*2; }
		if(BoxOrg->top >= SCENE_HEIGHT*BLOCK_SIZE+SCENE_Yo)  { BoxOrg->bottom = SCENE_HEIGHT*BLOCK_SIZE+SCENE_Yo -BLOCK_SIZE*2; BoxOrg->top = SCENE_HEIGHT*BLOCK_SIZE+SCENE_Yo -BLOCK_SIZE; }
		if(BoxOrg->left <= SCENE_Xo)  { BoxOrg->left = SCENE_Yo + BLOCK_SIZE; BoxOrg->right = SCENE_Yo + BLOCK_SIZE; }
		if(BoxOrg->right >= SCENE_WIDTH*BLOCK_SIZE+SCENE_Xo)  { BoxOrg->left = SCENE_WIDTH*BLOCK_SIZE+SCENE_Xo-BLOCK_SIZE; BoxOrg->right = SCENE_WIDTH*BLOCK_SIZE+SCENE_Xo-BLOCK_SIZE*2; }
		return HURT;
	}

	if (whatsThere(Box.left+BLOCK_SIZE/2,Box.top-21) == LOCKED_DOOR)numlockeddoors++;
	if (whatsThere(Box.left+BLOCK_SIZE/2,Box.top-21) == DOOR)numdoors++;
	if(exitingDoor){
		tiles[0] = whatsThere(Box.left+1,Box.top-1);
		tiles[1] = whatsThere(Box.right-1,Box.top-1);
	}
	for (int i = 0; i < 4; i++){
		if(tiles[i] == LOCKED_DOOR)numlockeddoors++;
		else if(tiles[i] == DOOR)numdoors++;
		else if(tiles[i] == WALKABLE)numwalkables++;
	}
	if(numlockeddoors>=2){
		return COLLIDES_LOCKED_DOOR;
	}
	if(!dungeon && numdoors>=2){
		if(exitingDoor){
			if(numwalkables + numdoors >= 4)return OK;
		}
		else{
			dungeon = true;
			exitingDoor = true;
			LoadLevel(prop[DUNGEON_PROP]?"dungeon":"market",false, data);
			BoxOrg->bottom = SCENE_Yo;
			BoxOrg->top=SCENE_Yo + BLOCK_SIZE;
			BoxOrg->left = SCENE_Xo + 7*BLOCK_SIZE;
			BoxOrg->right=BoxOrg->left+BLOCK_SIZE;
			if(prop[DUNGEON_PROP]){//dungeon
				int ki=0;
				bool loadTriforce = true;
				while(triforcesCollected[ki]!=nullptr && loadTriforce){
					if(strcmp(triforcesCollected[ki],id) == 0){
						loadTriforce=false;
					}
					ki++;
				}
				if(loadTriforce){
					boss = cBoss(data->GetID(IMG_BOSS));
					bossAlive = true;
				}
			}
			else{//market
				objects[0] = new cObject(SCENE_Xo+4*BLOCK_SIZE,SCENE_Yo + 5*BLOCK_SIZE,KEY);
				objects[0]->setCollectable(30);
				objects[1] = new cObject(SCENE_Xo+7*BLOCK_SIZE,SCENE_Yo + 6*BLOCK_SIZE,RED_HEART);
				objects[1]->setCollectable(15);
				int vector[] = {RED_HEART,BLUE_HEART};
				objects[1]->setAnimated(vector,2,FRAME_DELAY*2);
				objects[2] = new cObject(SCENE_Xo+10*BLOCK_SIZE,SCENE_Yo + 5*BLOCK_SIZE,HEART_CONTAINER);
				objects[2]->setCollectable(60);
				nObjects=3;
			}
			return OUTLIMITS;
		}
	}
	if(numwalkables == 4){
		if(!dungeon)exitingDoor = false;
		return OK;
	}
	return COLLIDES;
}

int cScene::whatsThere(int x,int y){
	for (int i = 0; i < nEnemies; i++) {
		if(enemies[i] != nullptr) {
			cRect target;
			enemies[i]->GetArea(&target);
			if (x >= target.left && x <=target.right && y >= target.bottom && y <= target.top) return HURT;
		}
	}
	if(bossAlive) {
		cRect target;
		boss.GetArea(&target);
		if (x >= target.left && x <=target.right && y >= target.bottom && y <= target.top) return HURT;
	}
	int bx,by;
	bx = (x-SCENE_Xo)/BLOCK_SIZE;
	by = (y-SCENE_Yo)/BLOCK_SIZE;
	if (bx < 0 || by < 0 || bx > SCENE_WIDTH ||by > SCENE_HEIGHT ) return OTHERS;
	if (isDoor(map[by*SCENE_WIDTH+bx])) return DOOR;
	if (isLockedDoor(map[by*SCENE_WIDTH+bx])) return LOCKED_DOOR;
	if (isWallkable(map[by*SCENE_WIDTH+bx])) return WALKABLE;
	if (isWater(map[by*SCENE_WIDTH+bx])) return WATER;
	if (isStairs(map[by*SCENE_WIDTH+bx]))return STAIRS;
	return OTHERS;
}

int cScene::whatsThereMonsterVersion(int x,int y){

	int bx,by;
	bx = (x-SCENE_Xo)/BLOCK_SIZE;
	by = (y-SCENE_Yo)/BLOCK_SIZE;
	if (bx < 0 || by < 0 || bx > SCENE_WIDTH ||by > SCENE_HEIGHT ) return OTHERS;
	if (isDoor(map[by*SCENE_WIDTH+bx])) return DOOR;
	if (isLockedDoor(map[by*SCENE_WIDTH+bx])) return LOCKED_DOOR;
	if (isWallkable(map[by*SCENE_WIDTH+bx])) return WALKABLE;
	if (isWater(map[by*SCENE_WIDTH+bx])) return WATER;
	return OTHERS;
}

void cScene::unlock(){
	for(int i=0;i<SCENE_HEIGHT*SCENE_WIDTH;i++){
		if (isLockedDoor(map[i])){
			map[i] = TILE_DOOR;
		}
	}
	generateCallLevel();
}

void cScene::LoadLevelAnimation(char *oldLevel, char *newLevel, int dir,int obj_id,int tex_id, cPlayer *Link, int link_id) {
	FILE *fdNew;
	char bufferNew[42];
	char coma;
	int i,j,tile;
	i=0;

	freeEnemies();
	freeObjects();

	sprintf(bufferNew,"%s%s%s",(char *)FILENAME,newLevel,(char *)FILENAME_EXT);

	fdNew=fopen(bufferNew,"r");

	int *newMap = (int *) malloc(sizeof(int)*SCENE_WIDTH*SCENE_HEIGHT);
	for(j=SCENE_HEIGHT-1;j>=0;j--) {
		for(i=0;i<SCENE_WIDTH;i++) {
			fscanf(fdNew,"%d",&tile);
			tile--;
			newMap[(j*SCENE_WIDTH)+i] = tile;
			fscanf(fdNew,"%c",&coma);//pass coma
		}
		fscanf(fdNew,"%c",&coma); //pass enter
	}

	switch(dir) {
	case DOWN:
		for (int k = SCENE_HEIGHT-1; k > 0; k--){
			for(i=SCENE_HEIGHT-2; i>=0; --i) {
				for(j=0; j<SCENE_WIDTH; ++j) {
					map[(i+1)*SCENE_WIDTH+j] = map[i*SCENE_WIDTH+j];
				}
			}
			for(j=0; j<SCENE_WIDTH; ++j) {
				map[j] = newMap[k*SCENE_WIDTH+j];
			}
			generateCallLevel();
			Draw(tex_id,obj_id,false,nullptr,0,0);
			int x, y; Link->GetPosition(&x, &y);
			Link->SetPosition(x,y+BLOCK_SIZE);
			Link->Draw(link_id, obj_id, false);
			glutSwapBuffers();
			Sleep(1000/SCENE_HEIGHT);
		}
		break;
	case LEFT:
		for(j = 0; j<SCENE_WIDTH-1; ++j) {
			for(int k=SCENE_WIDTH-2; k>=0; --k) {
				for(i = 0; i<SCENE_HEIGHT; ++i) {
					map[i*SCENE_WIDTH + k+1] = map[i*SCENE_WIDTH+k];
				}
			}

			for(i = 0; i<SCENE_HEIGHT; ++i) {
				map[i*SCENE_WIDTH] = newMap[i*SCENE_WIDTH + SCENE_WIDTH-j-1];
			}

			generateCallLevel();
			Draw(tex_id,obj_id,false,nullptr,0,0);
			int x, y; Link->GetPosition(&x, &y);
			Link->SetPosition(x+BLOCK_SIZE,y);
			Link->Draw(link_id, obj_id, false);
			glutSwapBuffers();
			Sleep(1000/SCENE_WIDTH);
		}
		break;
	case RIGHT:
		for(j = SCENE_WIDTH-1; j>0; --j) {
			for(int k=1; k<SCENE_WIDTH; ++k) {
				for(i = 0; i<SCENE_HEIGHT; ++i) {
					map[i*SCENE_WIDTH + k-1] = map[i*SCENE_WIDTH+k];
				}
			}
			for(i = 0; i<SCENE_HEIGHT; ++i) {
				map[i*SCENE_WIDTH+SCENE_WIDTH-1] = newMap[i*SCENE_WIDTH + (SCENE_WIDTH-1)-j];
			}

			generateCallLevel();
			Draw(tex_id,obj_id,false,nullptr,0,0);
			int x, y; Link->GetPosition(&x, &y);
			Link->SetPosition(x-BLOCK_SIZE,y);
			Link->Draw(link_id, obj_id, false);
			glutSwapBuffers();
			Sleep(1000/SCENE_WIDTH);
		}
		break;
	case UP:
		for (int k = 0; k < SCENE_HEIGHT-1; k++){
			for(i=1; i < SCENE_HEIGHT; i++) {
				for(j=0; j<SCENE_WIDTH; ++j) {
					map[(i-1)*SCENE_WIDTH+j] = map[i*SCENE_WIDTH+j];
				}
			}
			for(j=0; j<SCENE_WIDTH; ++j) {
				map[(i-1)*SCENE_WIDTH+j] = newMap[k*SCENE_WIDTH+j];
			}
			generateCallLevel();
			Draw(tex_id,obj_id,false,nullptr,0,0);
			int x, y; Link->GetPosition(&x, &y);
			Link->SetPosition(x,y-BLOCK_SIZE);
			Link->Draw(link_id, obj_id, false);
			glutSwapBuffers();
			Sleep(1000/SCENE_HEIGHT);
		}
		break;
	}
}

void cScene::drawEnemies() {
	for(int i = 0; i < nEnemies; ++i) {
		if(enemies[i] != nullptr)
			enemies[i]->draw();
	}
}

void cScene::freeEnemies() {
	for(int i = 0; i < nEnemies; ++i ) {
		if(enemies[i] != nullptr) {
			free(enemies[i]);
			enemies[i] = nullptr;
		}
	}
	nEnemies = 0;
}

void cScene::freeObjects() {
	for(int i = 0; i < nObjects; ++i ) {
		if(objects[i] != nullptr) {
			free(objects[i]);
			objects[i] = nullptr;
		}
	}
	nObjects = 0;
}

void cScene::processObjects(cPlayer *Link,int *n,int *vector){
	cRect linkBox ;
	bool alive;
	*n=0;
	Link->GetArea(&linkBox);
	for (int i = 0; i < nObjects; i++){
		if(objects[i] != nullptr){
			alive = objects[i]->process();
			if(objects[i]->isMovable()){
				cRect c = objects[i]->getArea();
				alive &= (whatsThereMonsterVersion(c.left,c.bottom) == WALKABLE);
				alive &= (whatsThereMonsterVersion(c.right,c.bottom) == WALKABLE);
				alive &= (whatsThereMonsterVersion(c.left,c.top) == WALKABLE);
				alive &= (whatsThereMonsterVersion(c.right,c.top) == WALKABLE);
			}
			if(objects[i]->collides(linkBox)){
				alive = false;
				if(objects[i]->isCollectable()){
					int price = objects[i]->getPrice();
					if(Link->getWealth() >= price){
						Link->paga(price);
						int antes,despues;
						switch (objects[i]->getId()){
						case HEART_CONTAINER:
							Link->incMaxLife();
							vector[*n]=HEART_CONTAINER;
							(*n)++;
							break;
						case KEY:
							Link->getKey();
							vector[*n]=KEY;
							(*n)++;
							break;
						case RUPY:
							vector[*n]=RUPY;
							(*n)++;
							break;
						case RUPY_X5:
							vector[*n]=RUPY;
							(*n)++;
							break;
						case FAIRY:
							vector[*n]=FAIRY;
							(*n)++;
							Link->heal(-1);
							//parar sonido corazon bajo
							break;
						case RED_HEART:
							antes = Link->getLife();
							despues = Link->heal(1);
							if (antes == despues){
								alive = true;
								Link->paga(-price);
							}
							else {
								vector[*n]=RED_HEART;
								(*n)++;
							}
							break;
						case TRIFORCE_Y:
							Link->acquireTriforce();
							vector[*n]=TRIFORCE_Y;
							(*n)++;
							break;
						default:
							break;
						}
					}
					else{//no puedo comprarlo
						alive = true;
					}
				}
				if(objects[i]->isMovable()){
					int num_cors = Link->damage(objects[i]->getDamage());
					Link->setGodMode(true);
					vector[*n]=ROCK;
					(*n)++;
				}
			}
			if(!alive) {
				free(objects[i]);
				objects[i] = nullptr;
			}
		}
	}
	cRect enemyBox;
	if(Link->swordThrown()){
		for (int i = 0; i < nEnemies; i++){
			if(enemies[i] != nullptr){
				enemies[i]->GetArea(&enemyBox);
				if(Link->hasMySwordHitAny(enemyBox)){
					if (enemies[i]->Damage() == 0){
						vector[*n]=SWORD_DOWN;
						(*n)++;
						int drop = enemies[i]->getDrop();
						if(drop!=-1){
							int posx,posy; enemies[i]->GetPosition(&posx, &posy);
							objects[nObjects] = new cObject(posx, posy, drop);
							objects[nObjects]->setCollectable(0);
							if(drop == RED_HEART) {
								int vector[] = {RED_HEART,BLUE_HEART};
								objects[nObjects]->setAnimated(vector,2,FRAME_DELAY*2);
								++nObjects;
							} else if(drop == RUPY) {
								int vector[] = {RUPY,RUPY_X5};
								objects[nObjects]->setCollectable(-1);
								objects[nObjects]->setAnimated(vector,2,FRAME_DELAY*2);
							}
							++nObjects;
						}
						free(enemies[i]);
						enemies[i] = nullptr;
					}
					Link->sayonaraSword();
				}
			}
		}
		if(bossAlive) {
			boss.GetArea(&enemyBox);
			if(Link->hasMySwordHitAny(enemyBox)){
				int bossD = boss.Damage();
				boss.setDamaged();
				if (bossD == 0){
					float r = 1.0, g = 0.0;
					for(int k = 0; k<50; ++k) {
						glColor3f(r,g,1.0);
						boss.draw();
						glColor3f(1.0,1.0,1.0);
						glutSwapBuffers();
						float aux = r; r=g; g=aux;
						Sleep(20);
					}
					objects[nObjects] = boss.dropTriforce();
					++nObjects;
					bossAlive=false;
					vector[*n]=SWORD_UP;
				}
				vector[*n]=SWORD_DOWN;
				(*n)++;
				Link->sayonaraSword();
			}
		}
	}
}

bool cScene::getBossAlive(){
	return bossAlive;
}

bool cScene::processAttacks(cRect swordBox) {
	cRect target;
	bool hitted = false;
	for (int i = 0; i < nEnemies && !hitted; i++) {
		if(enemies[i] != nullptr) {
			enemies[i]->GetArea(&target);
			if (collides(swordBox,target)){
				int drop = enemies[i]->getDrop();
				if(drop != -1){
					int posx,posy; enemies[i]->GetPosition(&posx, &posy);
					objects[nObjects] = new cObject(posx, posy, drop);
					objects[nObjects]->setCollectable(0);
					int vector[] = {0,0};
					switch (drop){
						case RED_HEART:
							vector[0] = RED_HEART;
							vector[1] = BLUE_HEART;
							objects[nObjects]->setAnimated(vector,2,FRAME_DELAY*2);
						break;
						case RUPY:
							vector[0] = RUPY;
							vector[1] = RUPY_X5;
							objects[nObjects]->setCollectable(-1);
							objects[nObjects]->setAnimated(vector,2,FRAME_DELAY*2);
						break;
						default:
							break;
					}
					++nObjects;
				}
					//Free memory
				free(enemies[i]);
				enemies[i] = nullptr;
				hitted = true;
			}
		}
	}
	if(bossAlive) {
		boss.GetArea(&target);
		if(collides(target,swordBox)){
			hitted = true;
			/*if (boss.Damage() == 0){
				objects[nObjects] = boss.dropTriforce();
				++nObjects;
				bossAlive=false;
			}*/
			int bossD = boss.Damage();
			boss.setDamaged();
			if (bossD == 0){
				float r = 1.0, g = 0.0;
				for(int k = 0; k<50; ++k) {
					glColor3f(r,g,1.0);
					boss.draw();
					glColor3f(1.0,1.0,1.0);
					glutSwapBuffers();
					float aux = r; r=g; g=aux;
					Sleep(20);
				}
			objects[nObjects] = boss.dropTriforce();
			++nObjects;
			bossAlive=false;
			}
		}
	}
	return hitted;
}

bool cScene::inDungeon() {
	return dungeon && prop[DUNGEON_PROP];
}

//void cScene::processAttacksDani(cRect swordBox) {
//	int right = swordBox.right-1, left = swordBox.left+1, bottom = swordBox.bottom+1, top = swordBox.top-1;
//	bool hitted = false;
//	//bottom-left corner
//	for (int i = 0; i < nEnemies && !hitted; i++) {
//		if(enemies[i] != nullptr) {
//			cRect target;
//			enemies[i]->GetArea(&target);
//			if (target.left >= left && target.left <= left+BLOCK_SIZE && target.bottom >= bottom && target.bottom <= bottom+BLOCK_SIZE) {
//				int restLife = enemies[i]->Damage();
//				if (restLife == 0) {
//					//get drop (if any)
//					int drop = enemies[i]->getDrop();
//					int posx,posy; enemies[i]->GetPosition(&posx, &posy);
//					if(drop == RED_HEART) {
//						objects[nObjects] = new cObject(posx, posy, RED_HEART);
//						int vector[] = {RED_HEART,BLUE_HEART};
//						objects[nObjects]->setCollectable(0);
//						objects[nObjects]->setAnimated(vector,2,FRAME_DELAY*2);
//						++nObjects;
//					} else if(drop == RUPY) {
//						objects[nObjects] = new cObject(posx, posy, RUPY);
//						int vector[] = {RUPY,RUPY_X5};
//						objects[nObjects]->setAnimated(vector,2,FRAME_DELAY*2);
//						objects[nObjects]->setCollectable(-1);
//						++nObjects;
//					}
//
//					//Free memory
//					free(enemies[i]);
//					enemies[i] = nullptr;
//					hitted = true;
//				}
//			}
//		}
//	}
//	//bottom-right corner
//	for (int i = 0; i < nEnemies && !hitted; i++) {
//		if(enemies[i] != nullptr) {
//			cRect target;
//			enemies[i]->GetArea(&target);
//			if (target.left >= right && target.left <= right+BLOCK_SIZE && target.bottom >= bottom && target.bottom <= bottom+BLOCK_SIZE) {
//				int restLife = enemies[i]->Damage();
//				if (restLife == 0) {
//					//get drop (if any)
//					int drop = enemies[i]->getDrop();
//					int posx,posy; enemies[i]->GetPosition(&posx, &posy);
//					if(drop == RED_HEART) {
//						objects[nObjects] = new cObject(posx, posy, RED_HEART);
//						int vector[] = {RED_HEART,BLUE_HEART};
//						objects[nObjects]->setCollectable(0);
//						objects[nObjects]->setAnimated(vector,2,FRAME_DELAY*2);
//						++nObjects;
//					} else if(drop == RUPY) {
//						objects[nObjects] = new cObject(posx, posy, RUPY);
//						int vector[] = {RUPY,RUPY_X5};
//						objects[nObjects]->setCollectable(-1);
//						objects[nObjects]->setAnimated(vector,2,FRAME_DELAY*2);
//						++nObjects;
//					}
//
//					free(enemies[i]);
//					enemies[i] = nullptr;
//					hitted = true;
//				}
//			}
//		}
//	}
//	//top-left corner
//	for (int i = 0; i < nEnemies && !hitted; i++) {
//		if(enemies[i] != nullptr) {
//			cRect target;
//			enemies[i]->GetArea(&target);
//			if (target.left >= left && target.left <= left+BLOCK_SIZE && target.bottom >= top && target.bottom <= top+BLOCK_SIZE) {
//				int restLife = enemies[i]->Damage();
//				if (restLife == 0) {
//					//get drop (if any)
//					int drop = enemies[i]->getDrop();
//					int posx,posy; enemies[i]->GetPosition(&posx, &posy);
//					if(drop == RED_HEART) {
//						objects[nObjects] = new cObject(posx, posy, RED_HEART);
//						int vector[] = {RED_HEART,BLUE_HEART};
//						objects[nObjects]->setAnimated(vector,2,FRAME_DELAY*2);
//						++nObjects;
//					} else if(drop == RUPY) {
//						objects[nObjects] = new cObject(posx, posy, RUPY);
//						int vector[] = {RUPY,RUPY_X5};
//						objects[nObjects]->setAnimated(vector,2,FRAME_DELAY*2);
//						++nObjects;
//					}
//
//					free(enemies[i]);
//					enemies[i] = nullptr;
//					hitted = true;
//				}
//			}
//		}
//	}
//	//top-right corner
//	for (int i = 0; i < nEnemies && !hitted; i++) {
//		if(enemies[i] != nullptr) {
//			cRect target;
//			enemies[i]->GetArea(&target);
//			if (target.left >= right && target.left <= right+BLOCK_SIZE && target.bottom >= top && target.bottom <= top+BLOCK_SIZE) {
//				int restLife = enemies[i]->Damage();
//				if (restLife == 0) {
//					//get drop (if any)
//					int drop = enemies[i]->getDrop();
//					int posx,posy; enemies[i]->GetPosition(&posx, &posy);
//					if(drop == RED_HEART) {
//						objects[nObjects] = new cObject(posx, posy, RED_HEART);
//						int vector[] = {RED_HEART,BLUE_HEART};
//						objects[nObjects]->setAnimated(vector,2,FRAME_DELAY*2);
//						++nObjects;
//					} else if(drop == RUPY) {
//						objects[nObjects] = new cObject(posx, posy, RUPY);
//						int vector[] = {RUPY,RUPY_X5};
//						objects[nObjects]->setAnimated(vector,2,FRAME_DELAY*2);
//						++nObjects;
//					}
//
//					free(enemies[i]);
//					enemies[i] = nullptr;
//					hitted = true;
//				}
//			}
//		}
//	}
//}