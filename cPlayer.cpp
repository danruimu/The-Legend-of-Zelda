#include "cPlayer.h"
#include <math.h>


cPlayer::cPlayer() {
	espasa = nullptr;
	life = 6;
	max_life = 6;
	points = 0;
	keys = 1;
	triforce = 0;
	max_triforces = 3;
	godMode = false;
	godModeDuration = 0;
}
cPlayer::~cPlayer(){}

void cPlayer::printInfo(int obj_id){
	int i;
	int pos = SCENE_Xo - 7;
	for (i = 0; i < max_life; i+=2){
		if (i+1 == life)DrawObject(obj_id,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE,HALF_HEART);
		else if(life > i) DrawObject(obj_id,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE,RED_HEART);
		else DrawObject(obj_id,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE,EMPTY_HEART);
		pos+=BLOCK_SIZE/2;
	}
	char *buffer = (char*)malloc(42);
	char *bufferLlaves = (char*)malloc(42);
	sprintf(buffer,"X%d",points);
	sprintf(bufferLlaves,"X%d",keys);
	pos = SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - (BLOCK_SIZE + strlen(buffer)*10);

	DrawObject(obj_id,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE,RUPY);
	pos+=BLOCK_SIZE;
	printText(pos,(SCENE_HEIGHT+1)*BLOCK_SIZE+(BLOCK_SIZE/4),buffer,GLUT_BITMAP_HELVETICA_18,1,1,1);
	pos += strlen(buffer)*10;
	DrawObject(obj_id,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE,KEY);
	pos+=BLOCK_SIZE-4;
	printText(pos,(SCENE_HEIGHT+1)*BLOCK_SIZE+(BLOCK_SIZE/4),bufferLlaves,GLUT_BITMAP_HELVETICA_18,1,1,1);

	free(buffer);
	free(bufferLlaves);

	pos = SCENE_Xo+SCENE_WIDTH*BLOCK_SIZE - (BLOCK_SIZE-BLOCK_SIZE/4)*max_triforces - 3;
	for(i=0;i<max_triforces;i++){
		if(triforce>i)DrawObject(obj_id,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE,TRIFORCE_Y);
		else
			DrawObject(obj_id,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE,TRIFORCE_B);
		pos += BLOCK_SIZE-BLOCK_SIZE/4;
	}
}

void cPlayer::Logic(bool pause){
	if (pause) return;
	if(espasa != nullptr && !espasa->process()){
		sayonaraSword();
	}
	if(godMode) {
		--godModeDuration;
		if(godModeDuration == 0) setGodMode(false);
	}
}

void cPlayer::Draw(int tex_id,int obj_id,bool trifuerza){
	float bordeX = (0.+LINK_SIZE)/LINK_SCENE_TEXTURES_WIDTH;
	float bordeY = (0.+LINK_SIZE)/LINK_SCENE_TEXTURES_HEIGHT;
	float blockX = (0.+LINK_SIZE)/LINK_SCENE_TEXTURES_WIDTH;
	float blockY = (0.+LINK_SIZE)/LINK_SCENE_TEXTURES_HEIGHT;
	int posx,posy;
	GetPosition(&posx,&posy);
	float xo,yo;
	if(trifuerza){
		anim_trifuerza++;
		yo= 150.0/185.0;
		DrawRect(tex_id,0,yo + blockY,blockX,yo);
		DrawObject(obj_id,posx,posy+BLOCK_SIZE-5,anim_trifuerza%2?TRIFORCE_B:TRIFORCE_Y);
		return;
	}else{
		anim_trifuerza = 0;
	}
	int direction = GetDirection();
	float state = GetState();
	xo = direction*(blockX+bordeX);
	yo = state*(blockY+bordeY);
	if(state == STATE_ATTACK_1 || state == STATE_ATTACK_2){
		NextFrame(STATE_IDLE,STATE_ATTACK_2,2*FRAME_DELAY);
	}
	if (state == STATE_ATTACK_2){
		switch (direction){
		case DIRECTION_DOWN:
			yo -= (0.+LINK_DESFASE_ATTACK)/LINK_SCENE_TEXTURES_HEIGHT;
			SetPosition(posx,posy-BLOCK_SIZE);
			yo+=blockY;
			DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
			yo-=blockY;
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
	if(godMode) {
		if(godModeDuration%2 == 0) glColor3f(1.0, 0.0, 0.0);
		else glColor3f(0.0, 1.0, 1.0);
	}
	DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
	if(godMode) {
		glColor3f(1.0, 1.0, 1.0);
	}
	if(espasa != nullptr){
		espasa->Render(obj_id);
	}
	printInfo(obj_id);
}

char* cPlayer::toString(){
	int posx,posy;
	GetPosition(&posx,&posy);
	char *buffer = (char*)malloc(42);
	sprintf(buffer,"%d,%d,%d,%d,%d,%d,%d",posx,posy,points,keys,triforce,life,max_life);
	return buffer;
}

void cPlayer::fromString(String data){
	int posx,posy;
	sscanf(data,"%d,%d,%d,%d,%d,%d,%d",&posx,&posy,&points,&keys,&triforce,&life,&max_life);
	SetPosition(posx,posy);
}
void cPlayer::DrawMuerto(int tex_id){
	float blockY = (0.+LINK_SIZE)/LINK_SCENE_TEXTURES_HEIGHT;
	float bordeX = (0.+LINK_SIZE)/LINK_SCENE_TEXTURES_WIDTH;
	float blockX = (0.+LINK_SIZE)/LINK_SCENE_TEXTURES_WIDTH;
	float col = (float)rand()/(float)RAND_MAX;
	glColor3f(1.,col,0.);
	float state = GetState();
	float xo = state*(blockX+bordeX);
	DrawRect(tex_id,xo,blockY,xo + blockX,0);
	NextFrame(0,4,FRAME_DELAY*2);
}

int cPlayer::ataca(){
	int posx,posy;
	int state = GetState();
	int direction = GetDirection();
	if (state != STATE_IDLE && state!=STATE_MOVE) return 0;
	GetPosition(&posx,&posy);
	SetState(STATE_ATTACK_1);
	switch(direction){
		case DIRECTION_UP:
			if(posy+SCENE_Yo > BLOCK_SIZE*SCENE_HEIGHT)return 0;
			if (life == max_life && espasa == nullptr) {
				espasa = new cObject(posx,posy+2*BLOCK_SIZE,SWORD_DOWN);
				espasa->setMovable(GetSpeed()*2,direction,1);
				return 2;
			}
			break;
		case DIRECTION_DOWN:
			if(posy-SCENE_Yo < 0)return 0;
			if (life == max_life && espasa == nullptr) {
				espasa = new cObject(posx,posy-2*BLOCK_SIZE,SWORD_DOWN);
				espasa->setMovable(GetSpeed()*2,direction,1);
				return 2;
			}
			break;
		case DIRECTION_RIGHT:
			if(posx+SCENE_Xo > BLOCK_SIZE*SCENE_WIDTH)return 0;
			if (life == max_life && espasa == nullptr) {
				espasa = new cObject(posx+2*BLOCK_SIZE,posy,SWORD_DOWN);
				espasa->setMovable(GetSpeed()*2,direction,1);
				return 2;
			}
			break;
		case DIRECTION_LEFT:
			if(posx-SCENE_Xo < 0)return 0;
			if (life == max_life && espasa == nullptr) {
				espasa = new cObject(posx-2*BLOCK_SIZE,posy,SWORD_DOWN);
				espasa->setMovable(GetSpeed()*2,direction,1);
				return 2;
			}
			break;
	}

	return 1;
}

int cPlayer::heal(int num_hearts){//-1 -> full recovery
	if(num_hearts == -1) life = max_life;
	else life = min(max_life,life+num_hearts);
	return life;
}

int cPlayer::getLife(){
	return life;
}

int cPlayer::damage(int num_hearts){ 
	life = max(0,life-num_hearts);
	return life;
}

void cPlayer::givePoints(int num_points){
	points+=num_points;
}

bool cPlayer::useKey(){
	if(keys > 0){
		keys--;
		return true;
	}
	return false;
	
}

void cPlayer::getKey(){
	keys++;
}

void cPlayer::sayonaraSword(){
	free(espasa);
	espasa = nullptr;
}

void cPlayer::setGodMode(bool mode) {
	if (mode) {
		godMode = true;
		godModeDuration = 60;
	} else {
		godMode = false;
	}
}

bool cPlayer::getGodMode() {
	return godMode;
}


cRect cPlayer::getSwordBox() {
	cRect swordBox, linkBox;
	GetArea(&linkBox);
	switch(GetDirection()) {
	case DIRECTION_DOWN:
		swordBox.bottom = linkBox.bottom - BLOCK_SIZE;
		swordBox.top = linkBox.top - BLOCK_SIZE;
		swordBox.right = linkBox.right;
		swordBox.left = linkBox.left;
		break;
	case DIRECTION_LEFT:
		swordBox.bottom = linkBox.bottom;
		swordBox.top = linkBox.top;
		swordBox.right = linkBox.right - BLOCK_SIZE;
		swordBox.left = linkBox.left - BLOCK_SIZE;
		break;
	case DIRECTION_UP:
		swordBox.bottom = linkBox.bottom + BLOCK_SIZE;
		swordBox.top = linkBox.top + BLOCK_SIZE;
		swordBox.right = linkBox.right;
		swordBox.left = linkBox.left;
		break;
	case DIRECTION_RIGHT:
		swordBox.bottom = linkBox.bottom;
		swordBox.top = linkBox.top;
		swordBox.right = linkBox.right + BLOCK_SIZE;
		swordBox.left = linkBox.left + BLOCK_SIZE;
		break;
	}
	return swordBox;
}

void cPlayer::paga(int price){
	points -= price;
}

int cPlayer::getWealth(){
	return points;
}

void cPlayer::incMaxLife(){
	max_life+=2;
}

void cPlayer::acquireTriforce(){
	triforce++;
}

bool cPlayer::swordThrown(){
	return espasa!=nullptr;
}

bool cPlayer::hasMySwordHitAny(cRect enemy){
	if(espasa==nullptr)return false;
	return espasa->collides(enemy);
}

int cPlayer::getHearts() {
	return life;
}

int cPlayer::getTriforces() {
	return triforce;
}
