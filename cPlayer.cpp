
#include "cPlayer.h"
#include <math.h>

cPlayer::cPlayer() {}
cPlayer::~cPlayer(){}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,despx,despy;
	despx = 1.0/PLAYER_TEXTURE_WIDTH;
	despy = -1.0/PLAYER_TEXTURE_HEIGHT;
	
	xo = ((float) (GetDirection())) /PLAYER_TEXTURE_WIDTH;
	yo = ((float) (GetState())) /PLAYER_TEXTURE_HEIGHT; 

	DrawRect(tex_id,xo,yo,xo + despx,yo + despy);
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

bool cPlayer::tirapalante(int* map){
	int x,y;
	int case1, case2;
	GetPosition(&x,&y);
	//TODO: cambio de mapa
	//TODO colisiones con enemigos
	switch (GetDirection()) {
	case DIRECTION_UP:
		if(x%BLOCK_SIZE == 0) {    // We are in a exactly one tile
			case1 = checkPos(x-BLOCK_SIZE, y, map);
			case2 = 2;	//true
		} else {
			case1 = checkPos(x-BLOCK_SIZE, y,map);
			case2 = checkPos(x, y,map);
		}
		if(case1 != 2 || case2 != 2) return false;

		y+=GetSpeed();
		if (y > SCENE_HEIGHT*BLOCK_SIZE) return false;
		break;
	case DIRECTION_DOWN:
		if(x%BLOCK_SIZE == 0) {    // We are in a exactly one tile
			case1 = checkPos(x-BLOCK_SIZE, y-BLOCK_SIZE-1, map);
			case2 = 2;
		} else {
			case1 = checkPos(x-BLOCK_SIZE, y-BLOCK_SIZE-1,map);
			case2 = checkPos(x, y-BLOCK_SIZE-1,map);
		}
		if(case1 != 2 || case2 != 2) return false;

		y-=GetSpeed();
		if (y < SCENE_Yo) return false;
		break;
	case DIRECTION_LEFT:
		if(y%BLOCK_SIZE == 0) {    // We are in a exactly one tile
			case1 = checkPos(x-BLOCK_SIZE-1, y-BLOCK_SIZE, map);
			case2 = 2;
		} else {
			case1 = checkPos(x-BLOCK_SIZE-1, y-BLOCK_SIZE,map);
			case2 = checkPos(x-BLOCK_SIZE-1, y,map);
		}
		if(case1 != 2 || case2 != 2) return false;

		x-=GetSpeed();
		if (x < SCENE_Xo) return false;
		break;
	case DIRECTION_RIGHT:
		if(y%BLOCK_SIZE == 0) {    // We are in a exactly one tile
			case1 = checkPos(x, y-BLOCK_SIZE, map);
			case2 = 2;
		} else {
			case1 = checkPos(x, y-BLOCK_SIZE,map);
			case2 = checkPos(x, y,map);
		}
		if(case1 != 2 || case2 != 2) return false;

		x+=GetSpeed();
		if (x > SCENE_WIDTH*BLOCK_SIZE) return false;
		break;
	default:
		break;
	}
	SetPosition(x,y);
	return true;
}
