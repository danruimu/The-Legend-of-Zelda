#include "cGame.h"
#include "Globals.h"

cGame::cGame(void)
{
	options.effectVolume = 1.0;
	options.musicVolume = 1.0;
}

cGame::~cGame(void)
{
}

bool cGame::startGame() {
	bool res=true;
	mainMenu = false;

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

	res = Data.LoadImage(IMG_OBJECTS,"sprites/treasures.png",GL_RGBA);
	if(!res) return false;

	Link.setAlive(true);
	Link.SetWidthHeight(BLOCK_SIZE,BLOCK_SIZE);
	Link.SetBlock(PLAYER_START_CX,PLAYER_START_CY);
	Link.SetState(STATE_IDLE);
	Link.SetDirection(DIRECTION_UP);
	Link.SetSpeed(STEP_LENGTH);	//MUST BE 32/x where x=y^2

	sound.playSound(sounds[LOZ_MUSIC_OVERWORLD]);

	return res;
}

bool cGame::Init()
{
	bool res=true;
	mainMenu = true;
	pause = false;
	up = true;
	nTransMM = 0;
	currentMM = 0;
	menuText[0] = "NEW GAME";
	menuText[1] = "OPTIONS";
	menuText[2] = "EXIT";
	currentOptMM = 0;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,GAME_WIDTH,0,GAME_HEIGHT,0,1);
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Main Menu initialization
	res = Data.LoadImage(IMG_MAINMENU, "sprites/main_menu.png", GL_RGBA);
	if(!res) return false;
	res = Scene.LoadMainMenu(currentMM);
	if(!res) return false;

	//Sounds Initialization
	sounds[LOZ_MUSIC_OVERWORLD] = sound.addSound("sounds/LOZ_MUSIC_Overworld_theme.wav", true);
	sounds[LOZ_MUSIC_MAIN_MENU] = sound.addSound("sounds/LOZ_MUSIC_Introduction.wav", true);
	sounds[LOZ_SWORD] = sound.addSound("sounds/LOZ_Sword.wav", false);
	sounds[LOZ_MUSIC_WHISTLE]= sound.addSound("sounds/LOZ_MUSIC_Whistle.wav", false);
	sounds[LOZ_SWORD_SHOOT]= sound.addSound("sounds/LOZ_Sword_Shoot.wav", false);
	sounds[LOZ_DIE] = sound.addSound("sounds/LOZ_Die.wav", false);
	sounds[LOZ_LOW_HEALTH] = sound.addSound("sounds/LOZ_LowHealth.wav", true);
	
	sound.playSound(sounds[LOZ_MUSIC_MAIN_MENU]);

	sound.setVolume(sounds[LOZ_MUSIC_OVERWORLD], options.musicVolume);
	sound.setVolume(sounds[LOZ_MUSIC_MAIN_MENU], options.musicVolume);
	sound.setVolume(sounds[LOZ_SWORD], options.effectVolume);
	sound.setVolume(sounds[LOZ_MUSIC_WHISTLE], options.musicVolume);
	sound.setVolume(sounds[LOZ_SWORD_SHOOT], options.effectVolume);
	sound.setVolume(sounds[LOZ_DIE], options.effectVolume);

	return res;
}

bool cGame::Loop()
{
	bool res=true;

	if(mainMenu) {
		if(up) {
			if(nTransMM == TRANS_MAINMENU) {
				currentMM++;
				if(currentMM == 4) {
					up = false;
					currentMM = 3;
				}
				res = Scene.LoadMainMenu(currentMM);
				if(!res) return false;
				nTransMM = 0;
			} else {
				nTransMM++;
			}
		} else {
			if(nTransMM == TRANS_MAINMENU) {
				currentMM--;
				if(currentMM == -1) {
					up = true;
					currentMM = 0;
				}
				res = Scene.LoadMainMenu(currentMM);
				if(!res) return false;
				nTransMM = 0;
			} else {
				nTransMM++;
			}
		}
	}

	int n = Link.getHearts();
	if (!mainMenu && n <= 2) sound.playSound(sounds[LOZ_LOW_HEALTH]);
	else sound.stopSound(sounds[LOZ_LOW_HEALTH]);

	sound.updateSound();

	res = Process();
	if(res && !pause) Render();

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

	if(!mainMenu && !pause) {
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
			if (Link.ataca()) {
				sound.playSound(sounds[LOZ_SWORD]);
			}
			return true;
		}
		
		if(keys[' ']) {
			keys[' '] = false;
			pause = true;
			Scene.setPaused();
			sound.pauseSound(sounds[LOZ_MUSIC_OVERWORLD]);
			return true;
		}
		//TODO: remove this 
		if(keys['q']) {
			keys['q'] = false;
			Link.addLife();
			return true;
		}
		if(keys['e']) {
			keys['e'] = false;
			Link.subLife();
			return true;
		}
		//TODO: to here
	} else if (mainMenu) {
		if(keys['s']) {
			keys['s'] = false;
			sound.playSound(sounds[LOZ_SWORD]);
			currentOptMM++;
			if(currentOptMM == 3) currentOptMM = 0;
		} else if(keys['w']) {
			keys['w'] = false;
			sound.playSound(sounds[LOZ_SWORD]);
			currentOptMM--;
			if(currentOptMM == -1) currentOptMM = 2;
		} else if(keys['j']) {
			keys['j'] = false;
			if(currentOptMM == 0) {    //NEW GAME
				sound.stopSound(sounds[LOZ_MUSIC_MAIN_MENU]);
				sound.playSound(sounds[LOZ_MUSIC_WHISTLE]);
				Scene.newGameAnimation(Data.GetID(IMG_MAINMENU));
				startGame();
			} else if (currentOptMM == 1) {   //OPTIONS
				sound.playSound(sounds[LOZ_SWORD]);
				//TODO: implementar opciones
			} else if (currentOptMM == 2) {    //EXIT
				sound.stopSound(sounds[LOZ_MUSIC_MAIN_MENU]);
				sound.playSound(sounds[LOZ_DIE]);
				Sleep(3000);
				return false;
			}
		}
	} else if(pause) {
		if(keys[' ']) {
			keys[' '] = false;
			pause = false;
			sound.resumeSound(sounds[LOZ_MUSIC_OVERWORLD]);
			return true;
		}
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

	if(!mainMenu) {
		Scene.Draw(Data.GetID(IMG_BLOCKS), mainMenu, NULL);
		Link.Draw(Data.GetID(IMG_PLAYER),Data.GetID(IMG_OBJECTS));
	} else Scene.Draw(Data.GetID(IMG_MAINMENU), mainMenu, menuText[currentOptMM]);

	for(i = 0; i < MAX_N_MONSTERS; ++i) {
		//if(monsters[i].isAlive()) monsters[i].Draw(Data.GetID(IMG_MONSTER);
	}

	glutSwapBuffers();
}
