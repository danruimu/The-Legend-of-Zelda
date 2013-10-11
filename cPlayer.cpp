
#include "cPlayer.h"
#include <math.h>

cPlayer::cPlayer() {}
cPlayer::~cPlayer(){}

void cPlayer::Draw(int tex_id)
{	
	int posx,posy;
	GetPosition(&posx,&posy);
	float bordeX = 15./351.;
	float bordeY = 15./185.;
	float blockX = 15./351.;
	float blockY = 15./185.;
	float xo,yo;
	float direction = GetDirection();
	float state = GetState();
	xo = direction*(blockX+bordeX);
	yo = state*(blockY+bordeY);
	if(state == STATE_ATTACK_1 || state == STATE_ATTACK_2){
		NextFrame(STATE_IDLE,STATE_ATTACK_2,2*FRAME_DELAY);
	}
	if (state == STATE_ATTACK_2){
		if(direction == DIRECTION_DOWN){
			SetPosition(posx,posy-BLOCK_SIZE);
			yo -= 6./185.;
			yo+=blockY;
			DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
			SetPosition(posx,posy);
			yo-=blockY;
		}
		if(direction == DIRECTION_UP){
			SetPosition(posx,posy+BLOCK_SIZE);
			yo += 6./185.;
			yo-=blockY;
			DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
			SetPosition(posx,posy);
			yo+=blockY;
		}
		if(direction == DIRECTION_LEFT){
			SetPosition(posx-BLOCK_SIZE,posy);
			xo+=6./351.;
			xo-=blockX;
			DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
			SetPosition(posx,posy);
			xo+=blockX;
		}
		if(direction == DIRECTION_RIGHT){
			SetPosition(posx+BLOCK_SIZE,posy);
			xo-=6./351.;
			xo+=blockX;
			DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
			SetPosition(posx,posy);
			xo-=blockX;
		}
	}
	DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
}

/* x = column
   y = row
   map = vector of tiles
*/
int checkPos(int x, int y, const int* map) {
	int bx,by;
	bx = x/BLOCK_SIZE;
	by = y/BLOCK_SIZE;
	return map[by*SCENE_WIDTH + bx ];
}

bool isWalkable(int tile) {
	if (tile == 2 || tile == 8 || tile == 14) return true;	//suelo
	
	return false;
}

// return
// true => same map
// false => change map
bool cPlayer::tirapalante(int* map){
	int x,y;
	int case1, case2;
	GetPosition(&x,&y);
	//TODO colisiones con enemigos
	switch (GetDirection()) {
	case DIRECTION_UP:
		if (y == SCENE_HEIGHT*BLOCK_SIZE) {
			SetPosition(x,SCENE_Yo);
			return false;
		}
		case1 = checkPos(x-BLOCK_SIZE, y, map);
		case2 = 2;
		if(x%BLOCK_SIZE != 0) {
			case2 = checkPos(x, y,map);
		}
		if ( !isWalkable(case1) || !isWalkable(case2)) return true;

		y+=GetSpeed();
		if (y > SCENE_HEIGHT*BLOCK_SIZE) return true;
		break;
	case DIRECTION_DOWN:
		if (y == SCENE_Yo) {
			SetPosition(x,BLOCK_SIZE*SCENE_HEIGHT);
			return false;
		}

		case1 = checkPos(x-BLOCK_SIZE, y-BLOCK_SIZE-1, map);
		case2 = 2;
		if(x%BLOCK_SIZE != 0) {
			case2 = checkPos(x, y-BLOCK_SIZE-1,map);
		}
		if ( !isWalkable(case1) || !isWalkable(case2)) return true;

		y-=GetSpeed();
		if (y < SCENE_Yo) return true;
		break;
	case DIRECTION_LEFT:
		if (x == SCENE_Xo) {
			SetPosition(BLOCK_SIZE*SCENE_WIDTH,y);
			return false;
		}

		case1 = checkPos(x-BLOCK_SIZE-1, y-BLOCK_SIZE, map);
		case2 = 2;
		if(y%BLOCK_SIZE != 0) {
			case2 = checkPos(x-BLOCK_SIZE-1, y,map);
		}
		if ( !isWalkable(case1) || !isWalkable(case2)) return true;

		x-=GetSpeed();
		if (x < SCENE_Xo) return true;
		break;
	case DIRECTION_RIGHT:
		if (x == BLOCK_SIZE*SCENE_WIDTH) {
			SetPosition(SCENE_Xo,y);
			return false;
		}

		case1 = checkPos(x, y-BLOCK_SIZE, map);
		case2 = 2;
		if(y%BLOCK_SIZE != 0) {
			case2 = checkPos(x, y,map);
		}
		if ( !isWalkable(case1) || !isWalkable(case2)) return true;

		x+=GetSpeed();
		if (x > SCENE_WIDTH*BLOCK_SIZE) return true;
		break;
	default:
		break;
	}
	SetPosition(x,y);
	NextFrame(STATE_MOVE,2,FRAME_DELAY);
	return true;
}
