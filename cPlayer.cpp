#include "cPlayer.h"
#include <math.h>


cPlayer::cPlayer() {
	espasa.alive =false;
	life = 6;
	max_life = 6;
	points = 0;
	keys = 3;
	triforce = 0;
	max_triforces = 3;
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
	pos += 5;
	DrawObject(obj_id,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE,RUPY);
	pos+=BLOCK_SIZE;
	char *buffer = (char*)malloc(42);
	sprintf(buffer,"X%d",points);
	printText(pos,(SCENE_HEIGHT+1)*BLOCK_SIZE+(BLOCK_SIZE/4),buffer,GLUT_BITMAP_HELVETICA_18,1,1,1);
	pos += strlen(buffer)*10;
	DrawObject(obj_id,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE,KEY);
	pos+=BLOCK_SIZE;
	sprintf(buffer,"X%d",keys);
	printText(pos,(SCENE_HEIGHT+1)*BLOCK_SIZE+(BLOCK_SIZE/4),buffer,GLUT_BITMAP_HELVETICA_18,1,1,1);
	pos += strlen(buffer)*10;
	free(buffer);
	for(i=0;i<max_triforces;i++){
		if(triforce>i)DrawObject(obj_id,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE,TRIFORCE_Y);
		else
			DrawObject(obj_id,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE,TRIFORCE_B);
		pos += BLOCK_SIZE-BLOCK_SIZE/4;
	}
}

void cPlayer::Logic(bool pause){
	if(espasa.alive && !pause){
		espasa.x+= espasa.direction==DIRECTION_RIGHT?STEP_LENGTH:0;
		espasa.x-= espasa.direction==DIRECTION_LEFT?STEP_LENGTH:0;
		espasa.y+= espasa.direction==DIRECTION_UP?STEP_LENGTH:0;
		espasa.y-= espasa.direction==DIRECTION_DOWN?STEP_LENGTH:0;
		if (espasa.x-BLOCK_SIZE <= 0 || espasa.y-BLOCK_SIZE<=0 || espasa.x >=BLOCK_SIZE*SCENE_WIDTH||espasa.y >= BLOCK_SIZE*SCENE_HEIGHT){
			espasa.alive = false;			
		}
	}
}

void cPlayer::Draw(int tex_id,int obj_id){	
	int posx,posy;
	GetPosition(&posx,&posy);
	float bordeX = (0.+LINK_SIZE)/LINK_SCENE_TEXTURES_WIDTH;
	float bordeY = (0.+LINK_SIZE)/LINK_SCENE_TEXTURES_HEIGHT;
	float blockX = (0.+LINK_SIZE)/LINK_SCENE_TEXTURES_WIDTH;
	float blockY = (0.+LINK_SIZE)/LINK_SCENE_TEXTURES_HEIGHT;
	float xo,yo;
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
	DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
	if(espasa.alive){
		DrawObject(obj_id,espasa.x,espasa.y,SWORD_DOWN+espasa.direction);
		//cambiarla de color
	}
	printInfo(obj_id);
}

int cPlayer::ataca(){
	int posx,posy;
	int state = GetState();
	int direction = GetDirection();
	if (!(state == STATE_IDLE || state==STATE_MOVE))return 0;
	GetPosition(&posx,&posy);
	switch(direction){
		case DIRECTION_UP:
			if(posy+BLOCK_SIZE > BLOCK_SIZE*SCENE_HEIGHT)return 0;
			if(!espasa.alive){
				espasa.direction = DIRECTION_UP;
				espasa.x=posx;
				espasa.y=posy+2*BLOCK_SIZE;
			}
			break;
		case DIRECTION_DOWN:
			if(posy-BLOCK_SIZE < 0)return 0;
			if(!espasa.alive){
				espasa.direction = DIRECTION_DOWN;
				espasa.x=posx;
				espasa.y=posy-2*BLOCK_SIZE;
			}
			break;
		case DIRECTION_RIGHT:
			if(posx+BLOCK_SIZE > BLOCK_SIZE*SCENE_WIDTH)return 0;
			if(!espasa.alive){
				espasa.direction = DIRECTION_RIGHT;
				espasa.x=posx+2*BLOCK_SIZE;
				espasa.y=posy;
			}
			break;
		case DIRECTION_LEFT:
			if(posx-BLOCK_SIZE < 0)return 0;
			if(!espasa.alive){
				espasa.direction = DIRECTION_LEFT;
				espasa.x=posx-2*BLOCK_SIZE;
				espasa.y=posy;
			}
			break;
	}
	SetState(STATE_ATTACK_1);

	if(!espasa.alive && life==max_life){
		espasa.alive = true;
		espasa.state=0;
		return 2;
	} else if(espasa.alive && life!=max_life) {
		espasa.alive = false;
		return 1;
	}
	return 1;
}

int cPlayer::heal(int num_hearts){//-1 -> full recovery
	if(num_hearts == -1) life = max_life;
	else life = min(max_life,life+num_hearts);
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

