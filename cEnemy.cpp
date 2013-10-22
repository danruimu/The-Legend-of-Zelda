#include "cEnemy.h"

void cEnemy::readEnemyProps(FILE *propFile) {
	char *readed = (char*) malloc(42);
	fscanf(propFile, "%s%d", readed, &maxLife);
	fscanf(propFile, "%s%d", readed, &damage);
	fscanf(propFile, "%d", &typeProyectile);
	fscanf(propFile, "%f", &probShoot);
	char *proy = (char*) malloc(42);
	fscanf(propFile, "%s%f%s%f%s%f",readed, &probDropHeart, proy, &probDropRuppe,proy,&probDropKey);
}

int cEnemy::shoot(){
	if(rand()/((float)RAND_MAX)<probShoot)return typeProyectile;
	return 0;
}

int cEnemy::getDamage(){
	return damage;
}


cEnemy::cEnemy(void)
{
	
}

cEnemy::cEnemy(int x, int y, char *type, int tex_id,int movementDelay) {
	this->movementDelay=movementDelay;
	movementSeq=0;
	this->maxLife = 1;
	this->life = maxLife;
	SetPosition(x,y);
	this->type = (char*) malloc(42);
	strcpy(this->type, type);
	SetState(ENEMY_STATE_IDLE);
	SetDirection(ENEMY_DOWN);
	this->tex_id = tex_id;
	this->changeDirectionDelay = 0;
	SetWidthHeight(BLOCK_SIZE, BLOCK_SIZE);

	int Ntype;
	Ntype = strcmp(type, OCTOROK_B);
	if (Ntype == 0) {
		FILE *propFile;
		propFile = fopen(FILE_PROP_OCTOROK_B, "r");
		if(propFile != NULL) {
			readEnemyProps(propFile);
			fclose(propFile);
		}
	}
	Ntype = strcmp(type, FAT_DOG_O);
	if(Ntype == 0) {
		FILE *propFile;
		propFile = fopen(FILE_PROP_FAT_DOG_O, "r");
		if(propFile != NULL) {
			readEnemyProps(propFile);
			fclose(propFile);
		}
	}
}

cEnemy::~cEnemy(void)
{
	free(type);
}

void cEnemy::draw() {
	int posx,posy;
	GetPosition(&posx,&posy);
	int direction = GetDirection(), state = GetState();
	Draw(tex_id,  direction, state,posx, posy, 2, 4);
}

void cEnemy::process() {
	int x,y;
	movementSeq++;
	if(movementSeq == movementDelay)movementSeq=0;
	if(movementSeq == 0){
		SetState(1-GetState());
		GetPosition(&x,&y);
		switch(GetDirection()){
			case ENEMY_DOWN:
				y-=GetSpeed();
				break;
			case ENEMY_UP:
				y+=GetSpeed();
				break;
			case ENEMY_RIGHT:
				x+=GetSpeed();
				break;
			case ENEMY_LEFT:
				x-=GetSpeed();
				break;
		}
		SetPosition(x,y);
	}
}

int cEnemy::getIA(){
	return IA;
}

void cEnemy::setIA(int IA){
	this->IA = IA;
}

int cEnemy::SetNewDirection() {
	++changeDirectionDelay;
	if(changeDirectionDelay == 60) {
		changeDirectionDelay = 0;
		switch(getIA()) {
		case RAND:
			int newDir;
			newDir = rand()%4;
			SetDirection(newDir);
			return newDir;
		}
	}
	return GetDirection();
}

int cEnemy::Damage() {
	life--;
	return life;
}

int cEnemy::getDrop() {
	bool heartDropped = false, ruppeDropped = false, keyDropped = false;
	float posHeart = (float)rand()/(float)RAND_MAX;
	float posRuppe = (float)rand()/(float)RAND_MAX;
	float posKey = (float)rand()/(float)RAND_MAX;
	if(posKey < probDropKey)return KEY;
	if(posHeart < probDropHeart)return RED_HEART;
	if(posRuppe < probDropRuppe)return RUPY;
	//if(probDropHeart > 0.0) {
	//	heartDropped = (probDropHeart - posHeart) > 0;
	//}
	//if(probDropRuppe > 0.0) {
	//	ruppeDropped = (probDropRuppe - posRuppe) > 0;
	//}
	//if(heartDropped && ruppeDropped) {
	//	if((probDropHeart - posHeart) >= (probDropRuppe - posRuppe)) { //drop heart
	//		return RED_HEART;
	//	} else { //drop ruppe
	//		return RUPY;
	//	}
	//} else if(heartDropped) {
	//	return RED_HEART;
	//} else if(ruppeDropped) {
	//	return RUPY;
	//}
	return -1;
}

void cEnemy::setMaxLife(int lifes) {
	this->maxLife = this->life = lifes;
}

void cEnemy::setDamage(int damage) {
	this->damage = damage;
}

void cEnemy::setTex_ID(int tex_id) {
	this->tex_id = tex_id;
}

void cEnemy::setMovementDelay(int movDelay) {
	this->movementDelay = movDelay;
}

void cEnemy::setMovementSeq(int moveSeq) {
	this->movementSeq = moveSeq;
}

int cEnemy::getTex_ID() {
	return tex_id;
}

int cEnemy::getMovementDelay() {
	return movementDelay;
}

int cEnemy::getMovementSeq() {
	return movementSeq;
}

void cEnemy::setType(char *type) {
	strcpy(this->type, type);
}
