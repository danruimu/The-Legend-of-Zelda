
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
	int auxy = SCENE_HEIGHT*BLOCK_SIZE;
	auxy -= y;
	auxy /= BLOCK_SIZE;
	int auxx = x/BLOCK_SIZE;
	return map[(y/BLOCK_SIZE)*SCENE_WIDTH + auxx ];
}

// x esta a la derecha de link
// y esta debajo de link
bool cPlayer::tirapalante(int* map){
	int x,y;
	int case1, case2;
	int bx,by,cont;
	GetPosition(&x,&y);
	//TODO: cambio de mapa
	//TODO colisiones con escenario
	//TODO colisiones con enemigos
	switch (GetDirection()) {
	case DIRECTION_UP:
		if(x%32 != 0) {
			case1 = checkPos(x-BLOCK_SIZE, y+BLOCK_SIZE-1, map);
			case2 = checkPos(x, y+BLOCK_SIZE-1, map);
		} else {
			case1 = checkPos(x-1, y+BLOCK_SIZE-1,map);
			case2 = 2;
		}
		if(case1 != 2 || case2 != 2) return false;
		y+=GetSpeed();
		if (y > SCENE_HEIGHT*BLOCK_SIZE) return false;
		break;
	case DIRECTION_DOWN:
		case1 = checkPos(x-BLOCK_SIZE, y-1,map);
		case2 = checkPos(x, y-BLOCK_SIZE-1,map);
		if (case1 != 2 || case2 != 2) return false;

		y-=GetSpeed();
		if (y < SCENE_Yo) return false;
		break;
	case DIRECTION_LEFT:
		x-=GetSpeed();

		if (x < SCENE_Xo) return false;
		/*if (checkPos(x, y, map) != 2 ) return false;
		if (checkPos(x, y+BLOCK_SIZE, map) != 2 ) return false;*/
		break;
	case DIRECTION_RIGHT:
		x+=GetSpeed();

		if (x > SCENE_WIDTH*BLOCK_SIZE) return false;
		/*if (checkPos(x+BLOCK_SIZE, y, map) != 2 ) return false;
		if (checkPos(x+BLOCK_SIZE, y+BLOCK_SIZE, map) != 2 ) return false;*/
		break;
	default:
		break;
	}
	SetPosition(x,y);
	return true;
}
