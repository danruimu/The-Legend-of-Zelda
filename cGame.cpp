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
	Link.SetState(STATE_IDLE);
	Link.SetDirection(0);
	Link.SetSpeed(BLOCK_SIZE/4);	//MUST BE 32/x where x=y^2
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
	int *map = Scene.GetMap();
	//Process Input
	if(keys[27])	
		return false;
	if(keys['e']) {
		keys['e'] = false;
		Link.SetState((Link.GetState()+1)%5);
		Link.Draw(Data.GetID(IMG_PLAYER));
		return true;
	}
	//TODO: comprovar que no salimos de los mapas que tenemos
	if(keys['w']) {
		keys['w'] = false;
		/*if(Link.GetDirection() == DIRECTION_UP)Link.tirapalante(map);*/
		Link.SetDirection(DIRECTION_UP);
		res = Link.tirapalante(map);
		if (!res) {
			char *level = (char*)malloc(4);
			strcpy(level, Scene.getId());
			level[1]--;
			if (!Scene.LoadLevel(level))return false;
			free(level);
		}
		if(Link.GetState() == STATE_IDLE) Link.SetState(STATE_MOVE);
		else Link.SetState(STATE_IDLE);
		Link.Draw(Data.GetID(IMG_PLAYER));
		return true;
	}
	if(keys['a']) {
		keys['a'] = false;
		/*if(Link.GetDirection() == DIRECTION_LEFT)Link.tirapalante(map);*/
		Link.SetDirection(DIRECTION_LEFT);
		res = Link.tirapalante(map);
		if (!res) {
			char *level = (char*)malloc(4);
			strcpy(level, Scene.getId());
			level[0]--;
			if (!Scene.LoadLevel(level))return false;
			free(level);
		}
		if(Link.GetState() == STATE_IDLE) Link.SetState(STATE_MOVE);
		else Link.SetState(STATE_IDLE);
		Link.Draw(Data.GetID(IMG_PLAYER));
		return true;
	}
	if(keys['s']) {
		keys['s'] = false;
		/*if(Link.GetDirection() == DIRECTION_DOWN)Link.tirapalante(map);*/
		Link.SetDirection(DIRECTION_DOWN);
		res = Link.tirapalante(map);
		if (!res) {
			char *level = (char*)malloc(4);
			strcpy(level, Scene.getId());
			level[1]++;
			if (!Scene.LoadLevel(level))return false;
			free(level);
		}
		if(Link.GetState() == STATE_IDLE) Link.SetState(STATE_MOVE);
		else Link.SetState(STATE_IDLE);
		Link.Draw(Data.GetID(IMG_PLAYER));
		return true;
	}
	if(keys['d']) {
		keys['d'] = false;
		/*if(Link.GetDirection() == DIRECTION_RIGHT)Link.tirapalante(map);*/
		Link.SetDirection(DIRECTION_RIGHT);
		res = Link.tirapalante(map);
		if (!res) {
			char *level = (char*)malloc(4);
			strcpy(level, Scene.getId());
			level[0]++;
			if (!Scene.LoadLevel(level))return false;
			free(level);
		}
		if(Link.GetState() == STATE_IDLE) Link.SetState(STATE_MOVE);
		else Link.SetState(STATE_IDLE);
		Link.Draw(Data.GetID(IMG_PLAYER));
		return true;
	}

	if(keys['j']) {
		keys['j'] = false;
		cPlayer linkSword = Link;
		int xo, yo; Link.GetPosition(&xo, &yo);

		PlaySound("sounds\\sword.wav",NULL,SND_FILENAME|SND_ASYNC);
		Link.SetState(STATE_ATTACK_1);
		Link.Draw(Data.GetID(IMG_PLAYER));
		Render();
		Sleep(50);
		Link.SetState(STATE_ATTACK_2);
		Render();
		Sleep(50);
		//Link.SetState(STATE_SWORD);
		LinkSword.SetWidthHeight(BLOCK_SIZE,BLOCK_SIZE);
		LinkSword.SetBlock(xo/BLOCK_SIZE,yo/BLOCK_SIZE);
		LinkSword.SetDirection(Link.GetDirection());
		LinkSword.SetState(STATE_SWORD);
		if (Link.GetDirection() == DIRECTION_UP) {
			LinkSword.SetPosition(xo, yo+BLOCK_SIZE);
		} else if (Link.GetDirection() == DIRECTION_DOWN) {
			LinkSword.SetPosition(xo, yo-BLOCK_SIZE);
		} else if (Link.GetDirection() == DIRECTION_RIGHT) {
			LinkSword.SetPosition(xo+BLOCK_SIZE, yo);
		} else {
			LinkSword.SetPosition(xo-BLOCK_SIZE, yo);
		}
		Render2();
		Sleep(100);
		Render();
		Link.SetState(STATE_ATTACK_2);
		Sleep(50);
		Render();
		Link.SetState(STATE_ATTACK_1);
		Sleep(50);
		Render();
		Link.SetState(STATE_IDLE);
		Sleep(50);
		Render();
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

void cGame::Render2()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	Scene.Draw(Data.GetID(IMG_BLOCKS));
	Link.Draw(Data.GetID(IMG_PLAYER));
	LinkSword.Draw(Data.GetID(IMG_PLAYER));

	glutSwapBuffers();
}
