
#include "cPlayer.h"

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

int checkPos(int x, int y, const int* map) {
	int bx,by,cont,auxby;
	bx = x/BLOCK_SIZE;
	auxby = y/BLOCK_SIZE;
	by = auxby;
	cont = map[bx + by * SCENE_WIDTH];
	return cont;
}

bool cPlayer::tirapalante(int* map){
	int x,y;
	int bx,by,cont;
	GetPosition(&x,&y);
	//TODO: cambio de mapa
	//TODO colisiones con escenario
	//TODO colisiones con enemigos
	switch (GetDirection()) {
	case DIRECTION_UP:
		y+=GetSpeed();
		if (y >= SCENE_HEIGHT*BLOCK_SIZE) return false;
		if (checkPos(x, y+BLOCK_SIZE, map) != 2 ) return false;
		if (checkPos(x+BLOCK_SIZE, y+BLOCK_SIZE, map) != 2 ) return false;
		break;
	case DIRECTION_DOWN:
		y-=GetSpeed();

		//if (y <= SCENE_Yo) return false;
		if (checkPos(x, y, map) != 2 ) return false;
		if (checkPos(x+BLOCK_SIZE, y, map) != 2 ) return false;
		break;
	case DIRECTION_LEFT:
		x-=GetSpeed();

		if (x <= SCENE_Xo) return false;
		if (checkPos(x, y, map) != 2 ) return false;
		if (checkPos(x, y+BLOCK_SIZE, map) != 2 ) return false;
		break;
	case DIRECTION_RIGHT:
		x+=GetSpeed();

		if (x >= SCENE_WIDTH*BLOCK_SIZE) return false;
		if (checkPos(x+BLOCK_SIZE, y, map) != 2 ) return false;
		if (checkPos(x+BLOCK_SIZE, y+BLOCK_SIZE, map) != 2 ) return false;
		break;
	default:
		break;
	}
	SetPosition(x,y);
	return true;
}
