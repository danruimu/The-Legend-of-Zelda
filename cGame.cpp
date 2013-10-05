#include "cGame.h"
#include "Globals.h"


cGame::cGame(void)
{
}

cGame::~cGame(void)
{
}

bool cGame::Init()
{
	bool res=true;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,GAME_WIDTH,0,GAME_HEIGHT,0,1);
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	res = Data.LoadImage(IMG_BLOCKS,"sprites/blocks.png",GL_RGBA);
	if(!res) return false;
	res = Scene.LoadLevel("H8");
	if(!res) return false;

	res = Data.LoadImage(IMG_PLAYER,"sprites/link.png",GL_RGBA);
	if(!res) return false;
	Link.SetWidthHeight(BLOCK_SIZE,BLOCK_SIZE);
	Link.SetBlock(PLAYER_START_CX,PLAYER_START_CY);
	Link.SetState(0);
	Link.SetDirection(0);
	return res;
}

bool cGame::Loop()
{
	bool res=true;

	res = Process();
	if(res) Render();

	return res;
}

void cGame::Finalize()
{
	exit(0);
}

//Input
void cGame::ReadKeyboard(unsigned char key, bool press)
{
	keys[key] = press;
}
void cGame::ReadSpecialKeyboard(unsigned char specialkey, bool press)
{
	specialKeys[specialkey] = press;
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
}

//Process
bool cGame::Process()
{

	bool res=true;
	
	//Process Input
	if(keys[27])	
		return false;
	if(keys['d']) {
		keys['d'] = false;
		Link.SetDirection((Link.GetDirection()+1)%4);
		Link.Draw(Data.GetID(IMG_PLAYER));
		return true;
	}
	if(keys['s']) {
		keys['s'] = false;
		Link.SetState((Link.GetState()+1)%4);
		Link.Draw(Data.GetID(IMG_PLAYER));
		return true;
	}

	
	//Game Logic
	//...

	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	Scene.Draw(Data.GetID(IMG_BLOCKS));
	Link.Draw(Data.GetID(IMG_PLAYER));

	glutSwapBuffers();
}
