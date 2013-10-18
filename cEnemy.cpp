#include "cEnemy.h"

void readEnemyProps(FILE *propFile, int *maxLife, int *damage, bool *proyectile, char *typeProyectile, float *probShoot, float *probDropHeart, float *probDropRuppe) {
	char *readed = (char*) malloc(42);
	fscanf(propFile, "%s%d", readed, maxLife);
	fscanf(propFile, "%s%d", readed, damage);
	fscanf(propFile, "%s%s%f", readed, typeProyectile, probShoot);
	if(strcmp(typeProyectile, "none") == 0) *proyectile = false;
	else *proyectile = true;
	char *proy = (char*) malloc(42);
	fscanf(propFile, "%s%f%s%f", readed, probDropHeart, proy, probDropRuppe);
}


cEnemy::cEnemy(void)
{
}

cEnemy::cEnemy(int x, int y, char *type) {
	this->maxLife = 1;
	this->life = maxLife;
	this->x = x;
	this->y = y;
	strcpy(this->type, type);
	this->state = ENEMY_STATE_IDLE;
	this->direction = ENEMY_DOWN;

	int Ntype;
	Ntype = strcmp(type, OCTOROK_B);
	if (Ntype == 0) {
		FILE *propFile;
		propFile = fopen(FILE_PROP_OCTOROK_B, "r");
		if(propFile != NULL) {
			readEnemyProps(propFile, &this->maxLife, &this->damage, &this->proyectile, this->typeProyectile, &this->probShoot, &this->probDropHeart, &this->probDropRuppe);
			fclose(propFile);
		}
		this->tex_id = IMG_ENEMY_OCTOROK_B;
	}
	Ntype = strcmp(type, FAT_DOG_O);
	if(Ntype == 0) {
		FILE *propFile;
		propFile = fopen(FILE_PROP_FAT_DOG_O, "r");
		if(propFile != NULL) {
			readEnemyProps(propFile, &this->maxLife, &this->damage, &this->proyectile, this->typeProyectile, &this->probShoot, &this->probDropHeart, &this->probDropRuppe);
			fclose(propFile);
		}
		this->tex_id = IMG_ENEMY_FAT_DOG_O;
	}
}

cEnemy::~cEnemy(void)
{
}

void cEnemy::draw() {
	int posx,posy;
	GetPosition(&posx,&posy);
	float bordeX = (0.+ENEMY_SIZE)/LINK_SCENE_TEXTURES_WIDTH;
	float bordeY = (0.+ENEMY_SIZE)/LINK_SCENE_TEXTURES_HEIGHT;
	float blockX = (0.+ENEMY_SIZE)/LINK_SCENE_TEXTURES_WIDTH;
	float blockY = (0.+ENEMY_SIZE)/LINK_SCENE_TEXTURES_HEIGHT;
	int direction = this->direction;
	int state = this->state;
	int xo, yo;
	xo = direction*(blockX+bordeX);
	yo = state*(blockY+bordeY);

	char *file;
	sprintf(file, "%s.png", this->type);

	switch (direction){
	case ENEMY_DOWN:
		DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
		break;
	case DIRECTION_UP:
		yo += (0.+LINK_DESFASE_ATTACK)/LINK_SCENE_TEXTURES_HEIGHT;
		SetPosition(posx,posy+BLOCK_SIZE);
		yo-=blockY;
		DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
		yo+=blockY;
		break;
	case DIRECTION_LEFT:
		xo+= (0.+LINK_DESFASE_ATTACK)/LINK_SCENE_TEXTURES_WIDTH;
		SetPosition(posx-BLOCK_SIZE,posy);
		xo-=blockX;
		DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
		xo+=blockX;
		break;
	case DIRECTION_RIGHT:
		xo-= (0.+LINK_DESFASE_ATTACK)/LINK_SCENE_TEXTURES_WIDTH;
		SetPosition(posx+BLOCK_SIZE,posy);
		xo+=blockX;
		DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
		xo-=blockX;
		break;
	}
	SetPosition(posx,posy);
}