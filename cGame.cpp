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
	FMOD_System_Create(&system);
	system.init(32, FMOD_INIT_NORMAL, 0);

	system.createSound("sounds/02_overworld_theme.wav", FMOD_HARDWARE, 0, &soundBack);
	soundBack->setMode(FMOD_LOOP_NORMAL);

	system->createSound("sounds/sword.wav", FMOD_HARDWARE, 0, &soundSword);
	soundSword->setMode(FMOD_LOOP_OFF);

	system->playSound(FMOD_CHANNEL, soundBack, false, 0);

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

	//Link Initialization
	res = Data.LoadImage(IMG_PLAYER,"sprites/link-org.png",GL_RGBA);
	if(!res) return false;

	res = Data.LoadImage(IMG_OBJECTS,"sprites/objects.png",GL_RGBA);
	if(!res) return false;

	Link.setAlive(true);
	Link.SetWidthHeight(BLOCK_SIZE,BLOCK_SIZE);
	Link.SetBlock(PLAYER_START_CX,PLAYER_START_CY);
	Link.SetState(STATE_IDLE);
	Link.SetDirection(DIRECTION_UP);
	Link.SetSpeed(STEP_LENGTH);	//MUST BE 32/x where x=y^2
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
	if(keys['w']) {
		keys['w'] = false;
		if (Link.GetDirection()!=DIRECTION_UP)
		Link.SetDirection(DIRECTION_UP);
		res = Link.tirapalante(map);
		if (!res) {
			char *level = (char*)malloc(4);
			strcpy(level, Scene.getId());
			level[1]--;
			if (!Scene.LoadLevel(level))return false;
			free(level);
		}
		return true;
	}
	if(keys['a']) {
		keys['a'] = false;
		if (Link.GetDirection()!=DIRECTION_LEFT)
		Link.SetDirection(DIRECTION_LEFT);
		res = Link.tirapalante(map);
		if (!res) {
			char *level = (char*)malloc(4);
			strcpy(level, Scene.getId());
			level[0]--;
			if (!Scene.LoadLevel(level))return false;
			free(level);
		}
		return true;
	}
	if(keys['s']) {
		keys['s'] = false;
		if (Link.GetDirection()!=DIRECTION_DOWN)
		Link.SetDirection(DIRECTION_DOWN);
		res = Link.tirapalante(map);
		if (!res) {
			char *level = (char*)malloc(4);
			strcpy(level, Scene.getId());
			level[1]++;
			if (!Scene.LoadLevel(level))return false;
			free(level);
		}
		return true;
	}
	if(keys['d']) {
		keys['d'] = false;
		if (Link.GetDirection()!=DIRECTION_RIGHT)
		Link.SetDirection(DIRECTION_RIGHT);
		res = Link.tirapalante(map);
		if (!res) {
			char *level = (char*)malloc(4);
			strcpy(level, Scene.getId());
			level[0]++;
			if (!Scene.LoadLevel(level))return false;
			free(level);
		}
		return true;
	}

	if(keys['j']) {
		keys['j'] = false;
		if (Link.ataca())PlaySound("sounds\\sword.wav",NULL,SND_FILENAME|SND_ASYNC|SND_NOSTOP);
		return true;
	}

	
	//Game Logic
	//...

	return res;
}

//Output
void cGame::Render()
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	Scene.Draw(Data.GetID(IMG_BLOCKS));
	Link.Draw(Data.GetID(IMG_PLAYER),Data.GetID(IMG_OBJECTS));

	for(i = 0; i < MAX_N_MONSTERS; ++i) {
		//if(monsters[i].isAlive()) monsters[i].Draw(Data.GetID(IMG_MONSTER);
	}

	glutSwapBuffers();
}
