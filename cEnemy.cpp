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
	this->x = x;
	this->y = y;
	this->type = (char*) malloc(42);
	strcpy(this->type, type);
	this->state = ENEMY_STATE_IDLE;
	this->direction = ENEMY_DOWN;
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
	
}