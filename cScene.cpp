#include "cScene.h"
#include "Globals.h"

cScene::cScene(void)
{
}

cScene::~cScene(void)
{
}

bool cScene::LoadLevel(int level)
{
	bool res;
	FILE *fd;
	char file[16];
	int i,j,px,py;
	char tile;
	float coordx_tile, coordy_tile;
	px=py=0;
	res=true;

	if(level<10) sprintf(file,"%s0%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf(file,"%s%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

	fd=fopen(file,"r");
	if(fd==NULL) return false;

	id_DL=glGenLists(1);
	glNewList(id_DL,GL_COMPILE);
		glBegin(GL_QUADS);
			float bordeX = (float)INTERTALE_SPACE/(TEXTURES_WIDTH*(TILE_SIZE+INTERTALE_SPACE));
			float bordeY = (float)INTERTALE_SPACE/(TEXTURES_HEIGHT*(TILE_SIZE+INTERTALE_SPACE));
			float blockX = (float)TILE_SIZE/(TEXTURES_WIDTH*(TILE_SIZE+INTERTALE_SPACE));
			float blockY = (float)TILE_SIZE/(TEXTURES_HEIGHT*(TILE_SIZE+INTERTALE_SPACE));
			int x,y;
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px = SCENE_Xo;
				py=SCENE_Yo+BLOCK_SIZE*j;
				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%c",&tile);
						map[(j*SCENE_WIDTH)+i] = tile - '0';
						x = map[(j*SCENE_WIDTH)+i]%TEXTURES_WIDTH;
						y = map[(j*SCENE_WIDTH)+i]/TEXTURES_WIDTH;
						coordx_tile = x*(blockX+bordeX);
						coordy_tile = y*(blockY+bordeY);

						glTexCoord2f(coordx_tile,coordy_tile+blockY);			glVertex2i(px           ,py           );
						glTexCoord2f(coordx_tile+blockX,coordy_tile+blockY);	glVertex2i(px+BLOCK_SIZE,py           );
						glTexCoord2f(coordx_tile+blockX,coordy_tile       );	glVertex2i(px+BLOCK_SIZE,py+BLOCK_SIZE);
						glTexCoord2f(coordx_tile       ,coordy_tile       );	glVertex2i(px           ,py+BLOCK_SIZE);
						px+=BLOCK_SIZE;
				}
				fscanf(fd,"%c",&tile); //pass enter
			}

		glEnd();
	glEndList();

	fclose(fd);

	return res;
}

void cScene::Draw(int tex_id)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
}
int* cScene::GetMap()
{
	return map;
}