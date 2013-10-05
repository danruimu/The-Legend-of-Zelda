
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
