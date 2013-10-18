#include "cEnemy.h"

void cEnemy::readEnemyProps(FILE *propFile) {
	char *readed = (char*) malloc(42);
	fscanf(propFile, "%s%d", readed, &maxLife);
	fscanf(propFile, "%s%d", readed, &damage);
	fscanf(propFile, "%s", typeProyectile);
	fscanf(propFile, "%f", &probShoot);
	if(strcmp(typeProyectile, "none") == 0) proyectile = false;
	else proyectile = true;
	char *proy = (char*) malloc(42);
	fscanf(propFile, "%s%f%s%f", readed, &probDropHeart, proy, &probDropRuppe);
}


cEnemy::cEnemy(void)
{
}

cEnemy::cEnemy(int x, int y, char *type) {
	this->maxLife = 1;
	this->life = maxLife;
	SetPosition(x,y);
	this->type = (char*) malloc(42);
	strcpy(this->type, type);
	SetState(ENEMY_STATE_IDLE);
	SetDirection(ENEMY_DOWN);
	this->typeProyectile = (char*) malloc(42);

	int Ntype;
	Ntype = strcmp(type, OCTOROK_B);
	if (Ntype == 0) {
		FILE *propFile;
		propFile = fopen(FILE_PROP_OCTOROK_B, "r");
		if(propFile != NULL) {
			readEnemyProps(propFile);
			fclose(propFile);
		}
		this->tex_id = IMG_ENEMY_OCTOROK_B;
	}
	Ntype = strcmp(type, FAT_DOG_O);
	if(Ntype == 0) {
		FILE *propFile;
		propFile = fopen(FILE_PROP_FAT_DOG_O, "r");
		if(propFile != NULL) {
			readEnemyProps(propFile);
			fclose(propFile);
		}
		this->tex_id = IMG_ENEMY_FAT_DOG_O;
	}
}

cEnemy::~cEnemy(void)
{
	free(type);
	free(typeProyectile);
}

void cEnemy::draw() {
	int posx,posy;
	GetPosition(&posx,&posy);
	float bordeX = (0.+ENEMY_SIZE)/ENEMY_TEXTURE_WIDTH;
	float bordeY = (0.+ENEMY_SIZE)/ENEMY_TEXTURE_HEIGHT;
	float blockX = (0.+ENEMY_SIZE)/ENEMY_TEXTURE_WIDTH;
	float blockY = (0.+ENEMY_SIZE)/ENEMY_TEXTURE_HEIGHT;
	float xo,yo;
	int direction = GetDirection(), state = GetState();
	xo = direction*(blockX+bordeX);
	yo = state*(blockY+bordeY);
	DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
}