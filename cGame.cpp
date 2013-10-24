#include "cGame.h"

cGame::cGame(void)
{
	//First view
	credits[0] = "CONGRATULATIONS!";
	credits[1] = "Enjoy this time and";
	credits[2] = "see the credits :-D";
	
	//Second view
	credits[3] = "DIRECTORS";
	credits[4] = "Dani Ruiz";
	credits[5] = "Oriol Vilarrubi";

	//Third view
	credits[6] = "DEVELOPERS";
	credits[7] = "Dani Ruiz";
	credits[8] = "Oriol Vilarrubi";

	//Fourth view
	credits[9] = "MUSIC OBTAINED FROM";
	credits[10] = "Dayjo";
	credits[11] = "https://http://noproblo.dayjo.org/";

	//Fifth view
	credits[12] = "SPECIAL THANKS";
	credits[13] = "Raquel Recio";
	credits[14] = "Coffe machine";

	//Sixth view
	credits[15] = "THANK YOU!";
	credits[16] = "Remember that donations";
	credits[17] = "will be very appreciated :-D";
}

cGame::~cGame(void){
	int i ;
	for (i= 0; i < 3; i++){
		free(menuText[i]);
	}
	i=0;
	while(unLockedLevels[i]!=nullptr){
		free(unLockedLevels[i++]);
	}
	while(triforcesCollected[i]!=nullptr){
		free(triforcesCollected[i++]);
	}
}

void cGame::drawInstructions(float r, float g, float b) {
	glClear(GL_COLOR_BUFFER_BIT);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*3, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE, "INSTRUCTIONS", GLUT_BITMAP_TIMES_ROMAN_24, r, g, b);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*2, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE*3, "W -> Move UP", GLUT_BITMAP_HELVETICA_12, r, g, b);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*2, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE*4, "S -> Move DOWN", GLUT_BITMAP_HELVETICA_12, r, g, b);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*2, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE*5, "A -> Move LEFT", GLUT_BITMAP_HELVETICA_12, r, g, b);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*2, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE*6, "D -> Move RIGHT", GLUT_BITMAP_HELVETICA_12, r, g, b);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*4, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE*7, "J or SPACE -> ATTACK/ENTER", GLUT_BITMAP_HELVETICA_12, r, g, b);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*3.5, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE*8, "ESCAPE -> PAUSE", GLUT_BITMAP_HELVETICA_12, r, g, b);

	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*3.5, SCENE_Yo + BLOCK_SIZE, "Press 'SPACE' key to skip", GLUT_BITMAP_HELVETICA_12, r, g, b);
	glutSwapBuffers();
}

void cGame::drawCredits(char *text[], float r, float g, float b) {
	glClear(GL_COLOR_BUFFER_BIT);
	
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*4, SCENE_Yo +SCENE_HEIGHT*BLOCK_SIZE/2 + BLOCK_SIZE*2, text[0], GLUT_BITMAP_TIMES_ROMAN_24, r, g, b);

	for(int i = 1; i<3; ++i) {
		printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*3, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE/2 + BLOCK_SIZE*(2-i), text[i], GLUT_BITMAP_HELVETICA_12, r, g, b);
	}

	glutSwapBuffers();
}

bool cGame::finalGame() {
	sound.stopSound(sounds[LOZ_MUSIC_OVERWORLD]);
	gameFinal = true;
	nSecCreditsDuration = FPS*30;
	sound.playSound(sounds[LOZ_MUSIC_UNDERWORLD]);
	actualCreditView = 0;
	char *text[3];
	text[0] = credits[actualCreditView];
	text[1] = credits[actualCreditView+1];
	text[2] = credits[actualCreditView+2];
	drawCredits(text, 0.0, 0.0, 0.0);

	return true;
}

bool cGame::startInstructions() {
	bool res = true;
	instructions = true;
	nSecInstructions = FPS*10;
	sound.playSound(sounds[LOZ_MUSIC_DEATH_MOUNTAIN]);

	/**********************************/
	/* DRAW INSTRUCTIONS              */
	drawInstructions(1.0, 1.0, 1.0);
	/**********************************/

	return res;
}

bool cGame::startGame() {
	bool res=true;
	mainMenu = false;
	pause = false;
	instructions = false;

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
	res = Scene.LoadLevel(Scene.getId(),false, &Data);
	if(!res) return false;

	res = Data.LoadImage(IMG_ENEMY_FAT_DOG_O, "sprites/fat-dog-o.png", GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_ENEMY_OCTOROK_B, "sprites/octorok-b.png", GL_RGBA);
	if(!res) return false;

	res = Data.LoadImage(IMG_BOSS, "sprites/pechotes.png", GL_RGBA);
	if(!res) return false;

	//Link Initialization
	res = Data.LoadImage(IMG_PLAYER,"sprites/link-org.png",GL_RGBA);
	if(!res) return false;

	res = Data.LoadImage(IMG_OBJECTS,"sprites/treasures.png",GL_RGBA);
	if(!res) return false;
	Link = cPlayer();
	Link.setAlive(true);
	Link.SetWidthHeight(BLOCK_SIZE,BLOCK_SIZE);
	Link.SetBlock(PLAYER_START_CX,PLAYER_START_CY);
	Link.SetState(STATE_IDLE);
	Link.SetDirection(DIRECTION_UP);
	Link.SetSpeed(STEP_LENGTH);	//MUST BE 32/x where x=y^2

	sound.playSound(sounds[LOZ_MUSIC_OVERWORLD]);

	return res;
}

void cGame::GameOver() {
	gameOver = true;
	Scene.freeEnemies();
	Scene.freeObjects();
	Link.sayonaraSword();
	/*Link = *(new cPlayer());*/
	Scene.setId("H8");
	mainMenu = false;
	sprintf(menuText[0],"NEW GAME");
	sprintf(menuText[1],"OPTIONS");
	sprintf(menuText[2],"EXIT");

	sound.stopSound(sounds[LOZ_MUSIC_OVERWORLD]);
	sound.stopSound(sounds[LOZ_MUSIC_UNDERWORLD]);
	sound.stopSound(sounds[LOZ_LOW_HEALTH]);
	sound.playSound(sounds[LOZ_MUSIC_GAME_OVER]);

	Link.SetBlock(SCENE_WIDTH/2,2);
	for (int i = 0; i < 30; i++){
		glClear(GL_COLOR_BUFFER_BIT);
		printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*3, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE/2, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24, 1.0, 1.0, 1.0);
		printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*4, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE, "PRESS 'SPACE KEY' TO RETURN TO MAIN MENU", GLUT_BITMAP_TIMES_ROMAN_10, 1.0, 1.0, 1.0);
		Link.DrawMuerto(Data.GetID(IMG_PLAYER));
		glutSwapBuffers();
		Sleep(100);
	}
}

bool cGame::Init()
{
	existsSavedGame = checkSavedGame();

	this->loadSettings();
	bool res=true;
	mainMenu = true;
	optMenu = false;
	pause = false;
	gameOver = false;
	instructions = false;
	gameFinal = false;
	up = true;
	nTransMM = 0;
	currentMM = 0;
	menuText[0] = (char*)malloc(42);
	menuText[1] = (char*)malloc(42);
	menuText[2] = (char*)malloc(42);
	sprintf(menuText[0],"NEW GAME");
	sprintf(menuText[1],"OPTIONS");
	sprintf(menuText[2],"EXIT");
	if(existsSavedGame) {
		menuText[3] = (char*)malloc(42);
		sprintf(menuText[3], "LOAD GAME");
	}
	currentOptMM = 0;
	currentPauseOpt = 0;

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

	for (int i = 0; i < NUM_MAX_GATES; i++){
		free(triforcesCollected[i]);
		free(unLockedLevels[i]);
		triforcesCollected[i] = nullptr;
		unLockedLevels[i] = nullptr;
	}

	//Sounds Initialization
	sounds[LOZ_MUSIC_OVERWORLD] = sound.addSound("sounds/LOZ_MUSIC_Overworld_theme.wav", true, MUSIC);
	sounds[LOZ_MUSIC_MAIN_MENU] = sound.addSound("sounds/LOZ_MUSIC_Introduction.wav", true, MUSIC);
	sounds[LOZ_SWORD] = sound.addSound("sounds/LOZ_Sword.wav", false, EFFECT);
	sounds[LOZ_SWORD_COMBINED] = sound.addSound("sounds/LOZ_Sword_Combined.wav", false, EFFECT);
	sounds[LOZ_MUSIC_WHISTLE]= sound.addSound("sounds/LOZ_MUSIC_Whistle.wav", false, MUSIC);
	sounds[LOZ_SWORD_SHOOT]= sound.addSound("sounds/LOZ_Sword_Shoot.wav", false, EFFECT);
	sounds[LOZ_DIE] = sound.addSound("sounds/LOZ_Die.wav", false, MUSIC);
	sounds[LOZ_LOW_HEALTH] = sound.addSound("sounds/LOZ_LowHealth.wav", true, EFFECT);
	sounds[LOZ_TEXT] = sound.addSound("sounds/LOZ_Text.wav", false, EFFECT);
	sounds[LOZ_MUSIC_DEATH_MOUNTAIN] = sound.addSound("sounds/LOZ_MUSIC_Death_mountain_dungeon.wav", true, MUSIC);
	sounds[LOZ_HIT] = sound.addSound("sounds/LOZ_Hit.wav", false, EFFECT);
	sounds[LOZ_HURT] = sound.addSound("sounds/LOZ_Hurt.wav", false, EFFECT);
	sounds[LOZ_GET_HEART] = sound.addSound("sounds/LOZ_Get_Heart.wav", false, EFFECT);
	sounds[LOZ_UNLOCK] = sound.addSound("sounds/LOZ_Unlock.wav", false, EFFECT);
	sounds[LOZ_MUSIC_GAME_OVER] = sound.addSound("sounds/LOZ_MUSIC_Game_Over.wav", true, MUSIC);
	sounds[LOZ_MUSIC_GET_TRIFORCE] = sound.addSound("sounds/LOZ_MUSIC_Get_Triforce.wav", false, MUSIC);
	sounds[LOZ_FANFARE] = sound.addSound("sounds/LOZ_Fanfare.wav", false, EFFECT);
	sounds[LOZ_MUSIC_UNDERWORLD] = sound.addSound("sounds/LOZ_MUSIC_Underworld_dungeon_theme.wav", true, MUSIC);
	sounds[LOZ_GET_RUPEE] = sound.addSound("sounds/LOZ_Get_Rupee.wav", false, EFFECT);
	sounds[LOZ_KEY] = sound.addSound("sounds/LOZ_Key.wav", false, EFFECT);
	sounds[LOZ_BOSS_SCREAM_1] = sound.addSound("sounds/LOZ_Boss_Scream1.wav", false, EFFECT);
	sounds[LOZ_BOSS_SCREAM_2] = sound.addSound("sounds/LOZ_Boss_Scream2.wav", false, EFFECT);
	
	sound.setVolume(MUSIC, options.musicVolume);
	sound.setVolume(EFFECT, options.effectVolume);

	sound.stopSound(sounds[LOZ_MUSIC_OVERWORLD]);
	sound.playSound(sounds[LOZ_MUSIC_MAIN_MENU]);

	return res;
}

bool cGame::Loop()
{
	bool res=true;

	if(Scene.inDungeon()) {
		sound.pauseSound(sounds[LOZ_MUSIC_OVERWORLD]);
		sound.playSound(sounds[LOZ_MUSIC_UNDERWORLD]);
	} else if(!pause && !mainMenu && !gameOver && !gameFinal) {
		sound.stopSound(sounds[LOZ_MUSIC_UNDERWORLD]);
		sound.resumeSound(sounds[LOZ_MUSIC_OVERWORLD]);
	}

	if(!instructions && !gameFinal) {
		res = Process();
		if(res && !gameFinal) Render();
	} else if(instructions) {
		if(keys[' ']) {
			keys[' '] = false;
			nSecInstructions = 1;
		}
		nSecInstructions--;
		drawInstructions(1.0, 1.0, 1.0);
		if(nSecInstructions==0) {
			sound.stopSound(sounds[LOZ_MUSIC_DEATH_MOUNTAIN]);
			sound.playSound(sounds[LOZ_TEXT]);
			for(float i = 1.0; i>=0; i-=0.05) {
				drawInstructions(i,i,i);
				Sleep(100);
			}
			res = startGame();
		}
	} else if(gameFinal) {
		--nSecCreditsDuration;
		if(nSecCreditsDuration%(FPS*5)==0 && nSecCreditsDuration != 0) {
			actualCreditView+=3;
			char *text[3];
			text[0] = credits[actualCreditView];
			text[1] = credits[actualCreditView+1];
			text[2] = credits[actualCreditView+2];
			drawCredits(text, 0.0, 0.0, 0.0);
		} else if(nSecCreditsDuration == 0) {
			Scene.freeEnemies();
			Scene.freeObjects();
			Link.sayonaraSword();
			Link = cPlayer();
			sound.stopSound(sounds[LOZ_MUSIC_UNDERWORLD]);
			sprintf(menuText[0],"NEW GAME");
			sprintf(menuText[1],"OPTIONS");
			sprintf(menuText[2],"EXIT");
			Init();
		} else {
			int aux = nSecCreditsDuration%(FPS*5);
			char *text[3];
			text[0] = credits[actualCreditView];
			text[1] = credits[actualCreditView+1];
			text[2] = credits[actualCreditView+2];
			float r,g,b;
			if(aux < 50) {
				r = g = b = ((float)aux*2)/100;
			} else if(aux > 250) {
				aux = 300 - aux;
				r = g = b = ((float)aux*2)/100;
			} else {
				r = g = b = 1.0;
			}
			drawCredits(text, r, g, b);
		}
	}

	return res;
}

void cGame::Finalize()
{
	this->saveSettings();
	Link.sayonaraSword();//liberar memoria
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


//TODO: savegame
bool cGame::mainMenuProcess(){
	if(keys['j'] || keys['J'] || keys['\r'] || keys[' ']) {
		keys['J'] = keys['j'] = keys['\r'] = keys[' '] = false;
		if (!optMenu){//we are in the main menu
			switch(currentOptMM){
			case NEW_GAME:
				sound.stopSound(sounds[LOZ_MUSIC_MAIN_MENU]);
				sound.playSound(sounds[LOZ_MUSIC_WHISTLE]);
				Scene.newGameAnimation(Data.GetID(IMG_MAINMENU),currentMM);
				startInstructions();
				break;
			case OPTIONS:
				sound.playSound(sounds[LOZ_SWORD]);
				optMenu = true;
				int vol,effect;
				vol = options.musicVolume*100.0;
				effect = options.effectVolume*100.0;
				/**************************************/
				// WORKAROUND
				if(vol%10 != 0) ++vol;
				if(effect%10 != 0) ++effect;
				/**************************************/
				sprintf(menuText[0],"^ MUSIC VOLUME v %d%%",vol);
				sprintf(menuText[1],"^ EFFECTS VOLUME v %d%%",effect);
				sprintf(menuText[2],"BACK");				
				return true;
				break;
			case LOAD_GAME:
				sound.stopSound(sounds[LOZ_MUSIC_MAIN_MENU]);
				sound.playSound(sounds[LOZ_MUSIC_WHISTLE]);
				Scene.newGameAnimation(Data.GetID(IMG_MAINMENU),currentMM);
				startGame();
				loadGame();
							
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
		else if(optMenu) {//we are in the options menu
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
	if(keys['d'] || keys['D'] || specialKeys[GLUT_KEY_RIGHT]) {
		keys['D'] = keys['d'] = specialKeys[GLUT_KEY_RIGHT] = false;
		sound.playSound(sounds[LOZ_TEXT]);
		currentOptMM++;
		if(!existsSavedGame || optMenu) currentOptMM = currentOptMM%3;
		else currentOptMM = currentOptMM%4;
		return true;
	}
	if(keys['a'] || keys['A'] || specialKeys[GLUT_KEY_LEFT]) {
		keys['A'] = keys['a'] = specialKeys[GLUT_KEY_LEFT] = false;
		sound.playSound(sounds[LOZ_TEXT]);
		currentOptMM--;
		if(!existsSavedGame || optMenu) currentOptMM = currentOptMM<0?2:currentOptMM;
		else currentOptMM = currentOptMM<0?3:currentOptMM;
		return true;
	}
	if (keys['w'] || keys['W'] || specialKeys[GLUT_KEY_UP]) {
		keys['W'] = keys['w'] = specialKeys[GLUT_KEY_UP] = false;
		if(optMenu) {//we are in the options menu
			sound.playSound(sounds[LOZ_TEXT]);
			if(currentOptMM == MUSIC_VOLUME) {
				int vol;
				options.musicVolume = min(1.0,options.musicVolume+0.1);
				sound.setVolume(MUSIC, options.musicVolume);
				vol = options.musicVolume*100.0;
				/**************************************/
				// WORKAROUND
				if(vol%10 != 0) ++vol;
				/**************************************/
				sprintf(menuText[0],"^ MUSIC VOLUME v %d%%",vol);
				return true;

			} 
			if(currentOptMM == EFFECTS_VOLUME) {
				int effect;
				options.effectVolume = min(1.0,options.effectVolume+0.1);
				sound.setVolume(EFFECT, options.effectVolume);
				effect = options.effectVolume*100.0;
				/**************************************/
				// WORKAROUND
				if(effect%10 != 0) ++effect;
				/**************************************/
				sprintf(menuText[1],"^ EFFECTS VOLUME v %d%%",effect);
				return true;
			}
		}
	}
	if (keys['S'] || keys['s'] || specialKeys[GLUT_KEY_DOWN]) {
			keys['S'] = keys['s'] = specialKeys[GLUT_KEY_DOWN] = false;
			if(optMenu) {//we are in the options menu
				sound.playSound(sounds[LOZ_TEXT]);
				if(currentOptMM == MUSIC_VOLUME) {
					int vol;
					options.musicVolume = max (0.0,options.musicVolume-0.1);
					sound.setVolume(MUSIC, options.musicVolume);
					vol = options.musicVolume*100.0;
					/**************************************/
					// WORKAROUND
					if(vol%10 != 0) ++vol;
					/**************************************/
					sprintf(menuText[0],"^ MUSIC VOLUME v %d%%",vol);
					return true;
				}
				if(currentOptMM == EFFECTS_VOLUME) {
					int effect;
					options.effectVolume = max (0.0,options.effectVolume-0.1);
					sound.setVolume(EFFECT, options.effectVolume);
					effect = options.effectVolume*100.0;
					/**************************************/
					// WORKAROUND
					if(effect%10 != 0) ++effect;
					/**************************************/
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
	sound.updateSound();
	
	//Game Logic
	cRect linkBox;
	Link.GetArea(&linkBox);
	Link.Logic(pause);
	int resProcessScene;
	if(pause) {
		if(specialKeys[GLUT_KEY_F5]){
			saveGame();
			return true;
		}
		if(keys[27]) {
			keys[27] = false;
			pause = false;
			sound.stopSound(sounds[LOZ_MUSIC_DEATH_MOUNTAIN]);
			sound.playSound(sounds[LOZ_TEXT]);
			sound.resumeSound(sounds[LOZ_MUSIC_OVERWORLD]);
			return true;
		} else if(keys['W'] || keys['w'] || specialKeys[GLUT_KEY_UP]) {
			keys['W'] = keys['w'] = specialKeys[GLUT_KEY_UP] = false;
			sound.stopSound(sounds[LOZ_MUSIC_DEATH_MOUNTAIN]);

			--currentPauseOpt;
			if(currentPauseOpt<0) currentPauseOpt = 2;

			int music = options.musicVolume*100.0, effect = options.effectVolume*100.0;
			/**************************************/
			// WORKAROUND
			if(music%10 != 0) ++music;
			if(effect%10 != 0) ++effect;
			/**************************************/
			sprintf(menuText[0], "<- MUSIC VOLUME -> %d%%", music);
			sprintf(menuText[1], "<- EFFECT VOLUME -> %d%%", effect);
			return true;
		} else if(keys['S'] || keys['s'] || specialKeys[GLUT_KEY_DOWN]) {
			keys['S'] = keys['s'] = specialKeys[GLUT_KEY_DOWN] = false;
			sound.stopSound(sounds[LOZ_MUSIC_DEATH_MOUNTAIN]);

			++currentPauseOpt;
			if(currentPauseOpt>2) currentPauseOpt=0;

			int music = options.musicVolume*100.0, effect = options.effectVolume*100.0;
			/**************************************/
			// WORKAROUND
			if(music%10 != 0) ++music;
			if(effect%10 != 0) ++effect;
			/**************************************/
			sprintf(menuText[0], "<- MUSIC VOLUME -> %d%%", music);
			sprintf(menuText[1], "<- EFFECT VOLUME -> %d%%", effect);
			return true;
		} else if(keys['A'] || keys['a'] || specialKeys[GLUT_KEY_LEFT]) {
			keys['A'] = keys['a'] = specialKeys[GLUT_KEY_LEFT] = false;
			if(currentPauseOpt==0) {   //MUSIC
				options.musicVolume = max(0.0,options.musicVolume-0.1);
				sound.setVolume(MUSIC, options.musicVolume);
				sound.playSound(sounds[LOZ_MUSIC_DEATH_MOUNTAIN]);
			} else if(currentPauseOpt==1) {    //EFFECT
				options.effectVolume = max(0.0,options.effectVolume-0.1);
				sound.setVolume(EFFECT, options.effectVolume);
			}
			if(currentPauseOpt!=2) {
				sound.playSound(sounds[LOZ_TEXT]);
				int music = options.musicVolume*100.0, effect = options.effectVolume*100.0;
				/**************************************/
				// WORKAROUND
				if(music%10 != 0) ++music;
				if(effect%10 != 0) ++effect;
				/**************************************/
				sprintf(menuText[0], "<- MUSIC VOLUME -> %d%%", music);
				sprintf(menuText[1], "<- EFFECT VOLUME -> %d%%", effect);
			}
			return true;
		} else if(keys['D'] || keys['d'] || specialKeys[GLUT_KEY_RIGHT]) {
			keys['D'] = keys['d'] = specialKeys[GLUT_KEY_RIGHT] = false;
			if(currentPauseOpt==0) {   //MUSIC
				options.musicVolume = min(1.0,options.musicVolume+0.1);
				sound.setVolume(MUSIC, options.musicVolume);
				sound.playSound(sounds[LOZ_MUSIC_DEATH_MOUNTAIN]);
			} else if(currentPauseOpt==1) {    //EFFECT
				options.effectVolume = min(1.0,options.effectVolume+0.1);
				sound.setVolume(EFFECT, options.effectVolume);
			}
			if(currentPauseOpt!=2) {
				sound.playSound(sounds[LOZ_TEXT]);
				int music = options.musicVolume*100.0, effect = options.effectVolume*100.0;
				/**************************************/
				// WORKAROUND
				if(music%10 != 0) ++music;
				if(effect%10 != 0) ++effect;
				/**************************************/
				sprintf(menuText[0], "<- MUSIC VOLUME -> %d%%", music);
				sprintf(menuText[1], "<- EFFECT VOLUME -> %d%%", effect);
			}
			return true;
		} else if(keys['J'] || keys['j'] || keys['\r']) {
			keys['J'] = keys['j'] = keys['\r'] = false;
			if(currentPauseOpt==2) {
				sound.stopSound(sounds[LOZ_MUSIC_MAIN_MENU]);
				sound.playSound(sounds[LOZ_DIE]);
				Sleep(3000);
				return false;
			}
			return true;
		}

	} else if(!mainMenu && !gameOver) {    //NOT IN PAUSE and NOT IN MAIN MENU
		if(specialKeys[GLUT_KEY_F5]){
			saveGame();
			return true;
		}
		int vector[MAX_N_MONSTERS];
		int n=MAX_N_MONSTERS;
		char* buffer;
		Scene.processObjects(&Link, &n, vector);
		for(int i=0; i<n; ++i) {
			switch(vector[i]) {
			case RED_HEART:
				sound.playSound(sounds[LOZ_GET_HEART]);
				if(Link.getHearts() > 2) {
					sound.stopSound(sounds[LOZ_LOW_HEALTH]);
				}
				break;
			case RUPY:
				sound.playSound(sounds[LOZ_GET_RUPEE]);
				break;
			case RUPY_X5:
				sound.playSound(sounds[LOZ_GET_RUPEE]);
				break;
			case KEY:
				sound.playSound(sounds[LOZ_KEY]);
				break;
			case TRIFORCE_Y:
				sound.pauseSound(sounds[LOZ_MUSIC_UNDERWORLD]);
				sound.playSound(sounds[LOZ_MUSIC_GET_TRIFORCE]);
				i=0;
				buffer = Scene.getId();
				while (triforcesCollected[i] != nullptr)i++;
				triforcesCollected[i] = (char*) malloc(3);
				triforcesCollected[i][0] = buffer[0];
				triforcesCollected[i][1] = buffer[1];
				triforcesCollected[i][2] = '\0';
				for (int i = 0; i < 90; i++){
					glClear(GL_COLOR_BUFFER_BIT);
					Link.Draw(Data.GetID(IMG_PLAYER),Data.GetID(IMG_OBJECTS),true);
					glutSwapBuffers();
					Sleep(100);
				}
				Link.heal(-1);
				sound.stopSound(sounds[LOZ_LOW_HEALTH]);
				sound.resumeSound(sounds[LOZ_MUSIC_OVERWORLD]);
				break;
			case ROCK:
				sound.playSound(sounds[LOZ_HURT]);
				if(Link.getHearts() <= 0){
					GameOver();
					return true;
				}
				if(Link.getHearts() <= 2) {
					sound.playSound(sounds[LOZ_LOW_HEALTH]);
				}
				break;
			case HEART_CONTAINER:
				sound.playSound(sounds[LOZ_FANFARE]);
				break;
			case SWORD_DOWN:
				if(!Scene.getBossAlive()) sound.playSound(sounds[LOZ_HIT]);
				else sound.playSound(sounds[LOZ_BOSS_SCREAM_1]);
				break;
			case SWORD_UP:
				sound.playSound(sounds[LOZ_BOSS_SCREAM_2]);
				break;
			}
		}
		if(keys[27]) {
			keys[27] = false;
			pause = true;
			sound.playSound(sounds[LOZ_TEXT]);
			sound.pauseSound(sounds[LOZ_MUSIC_OVERWORLD]);

			int music = options.musicVolume*100.0, effect = options.effectVolume*100.0;
			/**************************************/
			// WORKAROUND
			if(music%10 != 0) ++music;
			if(effect%10 != 0) ++effect;
			/**************************************/
			sprintf(menuText[0], "<- MUSIC VOLUME -> %d%%", music);
			sprintf(menuText[1], "<- EFFECT VOLUME -> %d%%", effect);
			return true;
		}
		if(keys['J'] || keys['j'] || keys[' ']) {
			keys['J'] = keys['j'] = keys[' '] = false;
			int ataca=Link.ataca();
			if (ataca==1) {
				sound.playSound(sounds[LOZ_SWORD]);
			}
			else if(ataca == 2){
				sound.playSound(sounds[LOZ_SWORD_COMBINED]);
			}

			cRect swordBox = Link.getSwordBox();
			if (Scene.processAttacks(swordBox)){
				if(!Scene.getBossAlive()) sound.playSound(sounds[LOZ_HIT]);
				else sound.playSound(sounds[LOZ_BOSS_SCREAM_1]);
			}
			
			return true;
		}

		/****************************************/
		// TO BE REMOVED
		if(keys[42]) {
			keys[42] = false;
			return finalGame();
		}
		/****************************************/

		if (keys['w']||keys['s']||keys['d']||keys['a']||keys['W']||keys['S']||keys['D']||keys['A']||specialKeys[GLUT_KEY_UP]||specialKeys[GLUT_KEY_DOWN]||specialKeys[GLUT_KEY_RIGHT]||specialKeys[GLUT_KEY_LEFT]){
			int speed;
			speed = Link.GetSpeed();
			if(keys['w']||keys['W']||specialKeys[GLUT_KEY_UP]) {
				if (Link.GetDirection()!=DIRECTION_UP)
				Link.SetDirection(DIRECTION_UP);
				linkBox.bottom+=speed;
				linkBox.top+=speed;
			} else if(keys['a']||keys['A']||specialKeys[GLUT_KEY_LEFT]) {
				if (Link.GetDirection()!=DIRECTION_LEFT)
				Link.SetDirection(DIRECTION_LEFT);
				linkBox.left-=speed;
				linkBox.right-=speed;
			} else if(keys['s']||keys['S']||specialKeys[GLUT_KEY_DOWN]) {
				if (Link.GetDirection()!=DIRECTION_DOWN)
				Link.SetDirection(DIRECTION_DOWN);
				linkBox.top-=speed;
				linkBox.bottom-=speed;
			} else if(keys['d']||keys['D']||specialKeys[GLUT_KEY_RIGHT]) {
				if (Link.GetDirection()!=DIRECTION_RIGHT)
				Link.SetDirection(DIRECTION_RIGHT);
				linkBox.left+=speed;
				linkBox.right+=speed;
			}
			switch (Scene.Process(&linkBox,unLockedLevels,triforcesCollected, &Data, &Link)){
				case OK:
					Link.SetArea(linkBox);
					Link.NextFrame(STATE_MOVE,2,FRAME_DELAY_WALK);
				break;
				case COLLIDES:
					return true;
				case COLLIDES_LOCKED_DOOR:
					if(Link.useKey()){
						int i = 0;
						while(unLockedLevels[i]!=nullptr)i++;
						unLockedLevels[i] = (char*)malloc(3);
						unLockedLevels[i][0] = Scene.getId()[0];
						unLockedLevels[i][1] = Scene.getId()[1];
						unLockedLevels[i][2] = '\0';
						Scene.unlock();
						sound.playSound(sounds[LOZ_UNLOCK]);
					}
				break;
				case OUTLIMITS:
					Link.sayonaraSword();
					Link.SetArea(linkBox);
				break;
				case HURT:
					Link.SetArea(linkBox);
					Link.NextFrame(STATE_MOVE,2,FRAME_DELAY_WALK);
					if(!Link.getGodMode()) {
						Link.setGodMode(true);
						sound.playSound(sounds[LOZ_HURT]);
						int restLifes = Link.damage(1);
						if(restLifes <= 0) {
							GameOver();
							Scene.setId("H8");
							return true;
						} else if(restLifes <= 2) {
							sound.playSound(sounds[LOZ_LOW_HEALTH]);
						}
					}
					break;
			}
			return true;
		} else {
			resProcessScene = Scene.Process(&linkBox,unLockedLevels, triforcesCollected,&Data, &Link);
			switch(resProcessScene) {
			case HURT:
				Link.SetArea(linkBox);
				Link.NextFrame(STATE_MOVE,2,FRAME_DELAY_WALK);
				if(!Link.getGodMode()) {
					Link.setGodMode(true);
					sound.playSound(sounds[LOZ_HURT]);
					int restLifes = Link.damage(1);
					if(restLifes <= 0) {
						GameOver();
						Scene.setId("H8");
						return true;
					}
				}
				break;
			}
		}
	}

	if(gameOver) {
		if(keys[' ']) {
			keys[' '] = false;
			sound.stopSound(sounds[LOZ_MUSIC_GAME_OVER]);
			Init();
		}
	}

	if (mainMenu){
		return mainMenuProcess();
	}
	


	return true;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();
	
	if(mainMenu) {
		if(up) {
			if(nTransMM == TRANS_MAINMENU) {
				currentMM++;
				if(currentMM == 4) {
					up = false;
					currentMM = 3;
				}
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
				nTransMM = 0;
			} else {
				nTransMM++;
			}
		}
		Scene.Draw(Data.GetID(IMG_MAINMENU),Data.GetID(IMG_OBJECTS), mainMenu, menuText, currentOptMM,currentMM, existsSavedGame && !optMenu);
	} else if (!gameOver) {//mainMenu= false
		Scene.Draw(Data.GetID(IMG_BLOCKS),Data.GetID(IMG_OBJECTS), mainMenu, NULL, 0,0,false);
		Link.Draw(Data.GetID(IMG_PLAYER),Data.GetID(IMG_OBJECTS),false);	
		if (pause)Scene.drawPauseMenu(menuText[0],menuText[1], menuText[2], currentPauseOpt);
	} else if(gameOver) {
		glClear(GL_COLOR_BUFFER_BIT);
		printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*3, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE/2, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24, 1.0, 1.0, 1.0);
		printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*4, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE, "PRESS 'SPACE KEY' TO RETURN TO MAIN MENU", GLUT_BITMAP_TIMES_ROMAN_10, 1.0, 1.0, 1.0);
		Link.DrawMuerto(Data.GetID(IMG_PLAYER));
	}

	glutSwapBuffers();
}

void cGame::saveSettings() {
	FILE *fd;
	char *file = (char*) malloc(42*10);
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, file);
	sprintf(file, "%s/The Legend of Zelda/options.txt", file);
	fd = fopen(file, "w+");
	char *buffer = (char *) malloc(42);
	sprintf(buffer, "%f\n%f\n", options.musicVolume, options.effectVolume); 
	if (fd != NULL) {
		fwrite(buffer, strlen(buffer), 1, fd);
		fclose(fd);
	}
	free(buffer);
}

void cGame::loadSettings() {
	FILE *fd;
	char *file = (char*) malloc(42*10);
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, file);
	sprintf(file, "%s/The Legend of Zelda/options.txt", file);
	fd = fopen(file, "r");
	if(fd != NULL) {
		fscanf(fd, "%f%f", &options.musicVolume, &options.effectVolume);
		fclose(fd);
	} else { //default
		options.musicVolume = 1;
		options.effectVolume = 1;
	}
}

bool cGame::checkSavedGame() {
	FILE* fd;
	char *file = (char*) malloc(42*10);
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, file);
	sprintf(file, "%s/The Legend of Zelda/save.loz", file);
	fd = fopen(file,"r");
	if(fd == nullptr) return false;
	fclose(fd);
	return true;
}


void cGame::saveGame(){
	if(Scene.isMarket()){
		FILE* fd;
		char *file = (char*) malloc(42*10);
		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, file);
		sprintf(file, "%s/The Legend of Zelda/save.loz", file);
		fd = fopen(file,"w");
		if(fd == nullptr)return;
		fprintf(fd,"%s %s",Scene.getId(),Link.toString());
		int i=0;
		while(unLockedLevels[i]!=nullptr){
			fprintf(fd," %s",unLockedLevels[i]);
			i++;
		}
		fprintf(fd," -");
		i=0;
		while(triforcesCollected[i]!=nullptr){
			fprintf(fd," %s",triforcesCollected[i]);
			i++;
		}
		fprintf(fd," - %s",Scene.getxDooryDoor()); 
		fclose(fd);
		Scene.setText("Game saved!");
		sound.playSound(sounds[LOZ_TEXT]);
		glutSwapBuffers();
	}
}

void cGame::loadGame(){
	FILE* fd;
	int x,y;
	Scene.loadMarket(&Data);
	char *file = (char*) malloc(42*10);
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, file);
	sprintf(file, "%s/The Legend of Zelda/save.loz", file);
	fd = fopen(file,"r");
	if(fd == nullptr)return;
	char* buffer = (char*)malloc(42);
	fscanf(fd,"%s",buffer);
	Scene.setId(buffer);
	fscanf(fd,"%s",buffer);
	Link.fromString(buffer);
	int i=0;
	fscanf(fd,"%s",buffer);
	while(strcmp(buffer,"-")!=0){
		unLockedLevels[i] = (String) malloc(42);
		strcpy(unLockedLevels[i],buffer);
		i++;
		fscanf(fd,"%s",buffer);
	}
	fscanf(fd,"%s",buffer);
	i=0;
	while(strcmp(buffer,"-")!=0){
		triforcesCollected[i] = (String) malloc(42);
		strcpy(triforcesCollected[i],buffer);
		i++;
		fscanf(fd,"%s",buffer);
	}
	fscanf(fd,"%d %d",&x,&y);
	Scene.setxDooryDoor(x,y);
	fclose(fd);
	Scene.setText("Game loaded!");
	sound.playSound(sounds[LOZ_TEXT]);
	glutSwapBuffers();
	Sleep(500);
}
