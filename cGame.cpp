#include "cGame.h"
#include "Globals.h"

cGame::cGame(void)
{
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
	this->loadSettings();
	bool res=true;
	mainMenu = true;
	optMenu = false;
	pause = false;
	up = true;
	nTransMM = 0;
	currentMM = 0;
	menuText[0] = (char*)malloc(42);
	menuText[1] = (char*)malloc(42);
	menuText[2] = (char*)malloc(42);
	sprintf(menuText[0],"NEW GAME");
	sprintf(menuText[1],"OPTIONS");
	sprintf(menuText[2],"EXIT");
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
	sounds[LOZ_MUSIC_OVERWORLD] = sound.addSound("sounds/LOZ_MUSIC_Overworld_theme.wav", true, MUSIC);
	sounds[LOZ_MUSIC_MAIN_MENU] = sound.addSound("sounds/LOZ_MUSIC_Introduction.wav", true, MUSIC);
	sounds[LOZ_SWORD] = sound.addSound("sounds/LOZ_Sword.wav", false, EFFECT);
	sounds[LOZ_MUSIC_WHISTLE]= sound.addSound("sounds/LOZ_MUSIC_Whistle.wav", false, MUSIC);
	sounds[LOZ_SWORD_SHOOT]= sound.addSound("sounds/LOZ_Sword_Shoot.wav", false, EFFECT);
	sounds[LOZ_DIE] = sound.addSound("sounds/LOZ_Die.wav", false, MUSIC);
	sounds[LOZ_LOW_HEALTH] = sound.addSound("sounds/LOZ_LowHealth.wav", true, EFFECT);
	sounds[LOZ_TEXT] = sound.addSound("sounds/LOZ_Text.wav", false, EFFECT);
	
	sound.setVolume(MUSIC, options.musicVolume);
	sound.setVolume(EFFECT, options.effectVolume);

	sound.playSound(sounds[LOZ_MUSIC_MAIN_MENU]);

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
	this->saveSettings();
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



bool cGame::mainMenuProcess(){
	if(keys['j'] || keys['\r']) {
		keys['j'] = keys['\r'] = false;
		if (!optMenu){//we are in the main menu
			switch(currentOptMM){
			case NEW_GAME:
				sound.stopSound(sounds[LOZ_MUSIC_MAIN_MENU]);
				sound.playSound(sounds[LOZ_MUSIC_WHISTLE]);
				Scene.newGameAnimation(Data.GetID(IMG_MAINMENU));
				startGame();
				break;
			case OPTIONS:
				sound.playSound(sounds[LOZ_SWORD]);
				optMenu = true;
				int vol,effect;
				vol = options.musicVolume*100.0;
				effect = options.effectVolume*100.0;
				sprintf(menuText[0],"^ MUSIC VOLUME v %d%%",vol);
				sprintf(menuText[1],"^ EFFECTS VOLUME v %d%%",effect);
				sprintf(menuText[2],"BACK");				
				return true;
				break;
			case EXIT:
				sound.stopSound(sounds[LOZ_MUSIC_MAIN_MENU]);
				sound.playSound(sounds[LOZ_DIE]);
				Sleep(3000);
				return false;
				break;
			}
		}
		else{//we are in the options menu
			if(currentOptMM == BACK){
				sound.playSound(sounds[LOZ_SWORD]);
				optMenu = false;
				sprintf(menuText[0],"NEW GAME");
				sprintf(menuText[1],"OPTIONS");
				sprintf(menuText[2],"EXIT");
				currentOptMM = 0;
				this->saveSettings();
			}
			return true;
		}
	}
	if(keys['d'] || specialKeys['f']) {
		keys['d'] = specialKeys['f'] = false;
		sound.playSound(sounds[LOZ_SWORD]);
		currentOptMM = (currentOptMM +1)%3;
		return true;
	}
	if(keys['a'] || specialKeys['d']) {
		keys['a'] = specialKeys['d'] = false;
		sound.playSound(sounds[LOZ_SWORD]);
		currentOptMM--;
		currentOptMM = currentOptMM<0?2:currentOptMM;
		return true;
	}
	if (keys['w'] || specialKeys['e']) {
		keys['w'] = specialKeys['e'] = false;
		if(optMenu) {//we are in the options menu
			sound.playSound(sounds[LOZ_TEXT]);
			if(currentOptMM == MUSIC_VOLUME) {
				int vol;
				options.musicVolume = min(1.0,options.musicVolume+0.1);
				sound.setVolume(MUSIC, options.musicVolume);
				vol = options.musicVolume*100.0;
				sprintf(menuText[0],"^ MUSIC VOLUME v %d%%",vol);
				return true;

			} 
			if(currentOptMM == EFFECTS_VOLUME) {
				int effect;
				options.effectVolume = min(1.0,options.effectVolume+0.1);
				sound.setVolume(EFFECT, options.effectVolume);
				effect = options.effectVolume*100.0;
				sprintf(menuText[1],"^ EFFECTS VOLUME v %d%%",effect);
				return true;
			}
		}
	}
	if (keys['s'] || specialKeys['g']) {
			keys['s'] = specialKeys['g'] = false;
			if(optMenu) {//we are in the options menu
				sound.playSound(sounds[LOZ_TEXT]);
				if(currentOptMM == MUSIC_VOLUME) {
					int vol;
					options.musicVolume = max (0.0,options.musicVolume-0.1);
					sound.setVolume(MUSIC, options.musicVolume);
					vol = options.musicVolume*100.0;
					sprintf(menuText[0],"^ MUSIC VOLUME v %d%%",vol);
					return true;
				}
				if(currentOptMM == EFFECTS_VOLUME) {
					int effect;
					options.effectVolume = max (0.0,options.effectVolume-0.1);
					sound.setVolume(EFFECT, options.effectVolume);
					effect = options.effectVolume*100.0;
					sprintf(menuText[1],"^ EFFECTS VOLUME v %d%%",effect);
					return true;
				}
			}
		}
	return true;
}

//Process
bool cGame::Process()
{
	
	//int *map = Scene.GetMap();

	if(keys[27])	
		return false;

	if(pause) {
		if(keys[' ']) {
			keys[' '] = false;
			pause = false;
			sound.playSound(sounds[LOZ_TEXT]);
			sound.resumeSound(sounds[LOZ_MUSIC_OVERWORLD]);
			return true;
		}
	}

	if (mainMenu){
		return mainMenuProcess();
	}

	if(keys[' ']) {
		keys[' '] = false;
		pause = true;
		sound.playSound(sounds[LOZ_TEXT]);
		sound.pauseSound(sounds[LOZ_MUSIC_OVERWORLD]);
		Scene.setPaused();
		return true;
	}

	if(keys['j']) {
		keys['j'] = false;
		if (Link.ataca()) {
			sound.playSound(sounds[LOZ_SWORD]);
		}
		return true;
	}
	if (keys['w']||keys['s']||keys['d']||keys['a']){
		int speed;
		cRect linkBox;
		Link.GetArea(&linkBox);
		speed = Link.GetSpeed();
		if(keys['w']) {
			keys['w'] = false;
			if (Link.GetDirection()!=DIRECTION_UP)
			Link.SetDirection(DIRECTION_UP);
			linkBox.bottom+=speed;
			linkBox.top+=speed;
			/*if (!Link.tirapalante(map)) {
				char *level = (char*)malloc(4);
				strcpy(level, Scene.getId());
				level[1]--;
				if (!Scene.LoadLevel(level))return false;
				free(level);
			}*/
		}
		if(keys['a']) {
			keys['a'] = false;
			if (Link.GetDirection()!=DIRECTION_LEFT)
			Link.SetDirection(DIRECTION_LEFT);
			linkBox.left-=speed;
			linkBox.right-=speed;
			/*if (!Link.tirapalante(map)) {
				char *level = (char*)malloc(4);
				strcpy(level, Scene.getId());
				level[0]--;
				if (!Scene.LoadLevel(level))return false;
				free(level);
			}*/
		}
		if(keys['s']) {
			keys['s'] = false;
			if (Link.GetDirection()!=DIRECTION_DOWN)
			Link.SetDirection(DIRECTION_DOWN);
			linkBox.top-=speed;
			linkBox.bottom-=speed;
			/*if (!Link.tirapalante(map)) {
				char *level = (char*)malloc(4);
				strcpy(level, Scene.getId());
				level[1]++;
				if (!Scene.LoadLevel(level))return false;
				free(level);
			}*/
		}
		if(keys['d']) {
			keys['d'] = false;
			if (Link.GetDirection()!=DIRECTION_RIGHT)
			Link.SetDirection(DIRECTION_RIGHT);
			linkBox.left+=speed;
			linkBox.right+=speed;
			/*if (!Link.tirapalante(map)) {
				char *level = (char*)malloc(4);
				strcpy(level, Scene.getId());
				level[0]++;
				if (!Scene.LoadLevel(level))return false;
				free(level);
			}*/
		}
		switch (Scene.Process(&linkBox)){
			case OK:
				Link.SetArea(linkBox);
				Link.NextFrame(STATE_MOVE,2,FRAME_DELAY);
			break;
			case COLLIDES:
				return true;
			case COLLIDES_LOCKED_DOOR:
				if(Link.useKey())
					Scene.unlock();
			break;
			case OUTLIMITS:
				Link.SetArea(linkBox);
			break;
		}
			return true;
	}

		
	//Game Logic
	//...

	return true;
}

//Output
void cGame::Render()
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	if(!mainMenu) {
		Scene.Draw(Data.GetID(IMG_BLOCKS), mainMenu, NULL, 0);
		Link.Draw(Data.GetID(IMG_PLAYER),Data.GetID(IMG_OBJECTS));
	} else Scene.Draw(Data.GetID(IMG_MAINMENU), mainMenu, menuText, currentOptMM);

	for(i = 0; i < MAX_N_MONSTERS; ++i) {
		//if(monsters[i].isAlive()) monsters[i].Draw(Data.GetID(IMG_MONSTER);
	}

	glutSwapBuffers();
}


void cGame::saveSettings() {
	FILE *fd = fopen(OPT_FILE, "w+");
	char *buffer = (char *) malloc(42);
	sprintf(buffer, "%f\n%f\n", options.musicVolume, options.effectVolume); 
	if (fd != NULL) {
		fwrite(buffer, strlen(buffer), 1, fd);
	}
}

void cGame::loadSettings() {
	FILE *fd = fopen(OPT_FILE, "r");
	if(fd != NULL) {
		fscanf(fd, "%f%f", &options.musicVolume, &options.effectVolume);
	} else { //default
		options.musicVolume = 1;
		options.effectVolume = 1;
	}
}
