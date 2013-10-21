#include "cGame.h"

cGame::cGame(void)
{
}

cGame::~cGame(void){
	int i ;
	for (i= 0; i < 3; i++){
		free(menuText[i]);
	}
	i=0;
	while(unLockedLevels[i]!=nullptr)
		free(unLockedLevels[i++]);
}

void cGame::drawInstructions(float r, float g, float b) {
	glClear(GL_COLOR_BUFFER_BIT);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*3, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE, "INSTRUCTIONS", GLUT_BITMAP_TIMES_ROMAN_24, r, g, b);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*3, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE*3, "W -> Move UP", GLUT_BITMAP_HELVETICA_12, r, g, b);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*3, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE*4, "S -> Move DOWN", GLUT_BITMAP_HELVETICA_12, r, g, b);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*3, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE*5, "A -> Move LEFT", GLUT_BITMAP_HELVETICA_12, r, g, b);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*3, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE*6, "D -> Move RIGHT", GLUT_BITMAP_HELVETICA_12, r, g, b);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*5, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE-BLOCK_SIZE*7, "J or SPACE -> ATTACK", GLUT_BITMAP_HELVETICA_12, r, g, b);
	glutSwapBuffers();
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
	res = Scene.LoadLevel("H8",false, &Data);
	if(!res) return false;

	res = Data.LoadImage(IMG_ENEMY_FAT_DOG_O, "sprites/fat-dog-o.png", GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_ENEMY_OCTOROK_B, "sprites/octorok-b.png", GL_RGBA);
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

void cGame::GameOver() {
	gameOver = true;
	Scene.freeEnemies();
	Scene.freeObjects();
	Link.sayonaraSword();
	Link = *(new cPlayer());
	mainMenu = false;
	sprintf(menuText[0],"NEW GAME");
	sprintf(menuText[1],"OPTIONS");
	sprintf(menuText[2],"EXIT");

	sound.stopSound(sounds[LOZ_MUSIC_OVERWORLD]);
	sound.stopSound(sounds[LOZ_LOW_HEALTH]);
	sound.playSound(sounds[LOZ_MUSIC_GAME_OVER]);
	glClear(GL_COLOR_BUFFER_BIT);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*3, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE/2, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24, 1.0, 1.0, 1.0);
	printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*5, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE, "PRESS 'SPACE KEY' TO RETURN TO MAIN MENU", GLUT_BITMAP_TIMES_ROMAN_10, 1.0, 1.0, 1.0);
	glutSwapBuffers();
	Sleep(100);
}

bool cGame::Init()
{
	this->loadSettings();
	bool res=true;
	mainMenu = true;
	optMenu = false;
	pause = false;
	gameOver = false;
	instructions = false;
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
	sounds[LOZ_HURT] = sound.addSound("sounds/LOZ_Hurt.wav", false, EFFECT);
	sounds[LOZ_GET_HEART] = sound.addSound("sounds/LOZ_Get_Heart.wav", false, EFFECT);
	sounds[LOZ_UNLOCK] = sound.addSound("sounds/LOZ_Unlock.wav", false, EFFECT);
	sounds[LOZ_MUSIC_GAME_OVER] = sound.addSound("sounds/LOZ_MUSIC_Game_Over.wav", true, MUSIC);
	
	sound.setVolume(MUSIC, options.musicVolume);
	sound.setVolume(EFFECT, options.effectVolume);

	sound.playSound(sounds[LOZ_MUSIC_MAIN_MENU]);

	return res;
}

bool cGame::Loop()
{
	bool res=true;

	if(!instructions) {
		res = Process();
		if(res) Render();
	} else {
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



bool cGame::mainMenuProcess(){
	if(keys['j'] || keys['\r']) {
		keys['j'] = keys['\r'] = false;
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
	if(keys['d'] || specialKeys['f']) {
		keys['d'] = specialKeys['f'] = false;
		sound.playSound(sounds[LOZ_TEXT]);
		currentOptMM = (currentOptMM +1)%3;
		return true;
	}
	if(keys['a'] || specialKeys['d']) {
		keys['a'] = specialKeys['d'] = false;
		sound.playSound(sounds[LOZ_TEXT]);
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
	if (keys['s'] || specialKeys['g']) {
			keys['s'] = specialKeys['g'] = false;
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
		if(keys[27]) {
			keys[27] = false;
			pause = false;
			sound.stopSound(sounds[LOZ_MUSIC_DEATH_MOUNTAIN]);
			sound.playSound(sounds[LOZ_TEXT]);
			sound.resumeSound(sounds[LOZ_MUSIC_OVERWORLD]);
			return true;
		} else if(keys['w'] || specialKeys['e']) {
			keys['w'] = specialKeys['e'] = false;
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
		} else if(keys['s'] || specialKeys['g']) {
			keys['s'] = specialKeys['g'] = false;
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
		} else if(keys['a'] || specialKeys['d']) {
			keys['a'] = specialKeys['d'] = false;
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
		} else if(keys['d'] || specialKeys['f']) {
			keys['d'] = specialKeys['f'] = false;
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
		} else if(keys['j'] || keys['\r']) {
			keys['j'] = keys['\r'] = false;
			if(currentPauseOpt==2) {
				sound.stopSound(sounds[LOZ_MUSIC_MAIN_MENU]);
				sound.playSound(sounds[LOZ_DIE]);
				Sleep(3000);
				return false;
			}
			return true;
		}

	} else if(!mainMenu && !gameOver) {    //NOT IN PAUSE and NOT IN MAIN MENU
		Scene.processObjects(&Link);
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
		if(keys['j'] || keys[' ']) {
			keys['j'] = keys[' '] = false;
			int ataca=Link.ataca();
			if (ataca==1) {
				sound.playSound(sounds[LOZ_SWORD]);
			}
			else if(ataca == 2){
				sound.playSound(sounds[LOZ_SWORD_COMBINED]);
			}

			cRect swordBox = Link.getSwordBox();
			Scene.processAttacks(swordBox);
			
			return true;
		}

		/****************************************/
		// TO BE REMOVED
		if(keys['q']) {
			keys['q'] = false;
			int lifes = Link.heal(1);
			sound.playSound(sounds[LOZ_GET_HEART]);
			if(lifes <= 2) {
				sound.playSound(sounds[LOZ_LOW_HEALTH]);
			} else {
				sound.stopSound(sounds[LOZ_LOW_HEALTH]);
			}
			return true;
		}
		if(keys['e']) {
			keys['e'] = false;
			int lifes = Link.damage(1);
			sound.playSound(sounds[LOZ_HURT]);
			if(lifes <= 2 && lifes > 0) {
				sound.playSound(sounds[LOZ_LOW_HEALTH]);
			} else if(lifes == 0) {
				GameOver();
				return true;
			} else {
				sound.stopSound(sounds[LOZ_LOW_HEALTH]);
			} 
			return true;
		}
		/****************************************/

		if (keys['w']||keys['s']||keys['d']||keys['a']){
			int speed;
			speed = Link.GetSpeed();
			if(keys['w']) {
				if (Link.GetDirection()!=DIRECTION_UP)
				Link.SetDirection(DIRECTION_UP);
				linkBox.bottom+=speed;
				linkBox.top+=speed;
			} else if(keys['a']) {
				if (Link.GetDirection()!=DIRECTION_LEFT)
				Link.SetDirection(DIRECTION_LEFT);
				linkBox.left-=speed;
				linkBox.right-=speed;
			} else if(keys['s']) {
				if (Link.GetDirection()!=DIRECTION_DOWN)
				Link.SetDirection(DIRECTION_DOWN);
				linkBox.top-=speed;
				linkBox.bottom-=speed;
			} else if(keys['d']) {
				if (Link.GetDirection()!=DIRECTION_RIGHT)
				Link.SetDirection(DIRECTION_RIGHT);
				linkBox.left+=speed;
				linkBox.right+=speed;
			}
			switch (Scene.Process(&linkBox,unLockedLevels, &Data)){
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
						if(restLifes == 0) {
							GameOver();
							return true;
						} else if(restLifes <= 2) {
							sound.playSound(sounds[LOZ_LOW_HEALTH]);
						}
					}
					break;
			}
			return true;
		} else {
			resProcessScene = Scene.Process(&linkBox,unLockedLevels, &Data);
			switch(resProcessScene) {
			case HURT:
				Link.SetArea(linkBox);
				Link.NextFrame(STATE_MOVE,2,FRAME_DELAY_WALK);
				if(!Link.getGodMode()) {
					Link.setGodMode(true);
					sound.playSound(sounds[LOZ_HURT]);
					int restLifes = Link.damage(1);
					if(restLifes == 0) {
						GameOver();
						return true;
					}
				}
				break;
			}
		}
	}

	if(gameOver) {
		if(keys[' ']) {
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
		Scene.Draw(Data.GetID(IMG_MAINMENU),Data.GetID(IMG_OBJECTS), mainMenu, menuText, currentOptMM,currentMM);
	} else if (!gameOver) {//mainMenu= false
		Scene.Draw(Data.GetID(IMG_BLOCKS),Data.GetID(IMG_OBJECTS), mainMenu, NULL, 0,0);
		Link.Draw(Data.GetID(IMG_PLAYER),Data.GetID(IMG_OBJECTS));
		if (pause)Scene.drawPauseMenu(menuText[0],menuText[1], menuText[2], currentPauseOpt);
	} else if(gameOver) {
		glClear(GL_COLOR_BUFFER_BIT);
		printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*3, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE/2, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24, 1.0, 1.0, 1.0);
		printText(SCENE_Xo + SCENE_WIDTH*BLOCK_SIZE/2 - BLOCK_SIZE*5, SCENE_Yo + SCENE_HEIGHT*BLOCK_SIZE/2-BLOCK_SIZE, "PRESS 'SPACE KEY' TO RETURN TO MAIN MENU", GLUT_BITMAP_TIMES_ROMAN_10, 1.0, 1.0, 1.0);
	}

	glutSwapBuffers();
}

void cGame::saveSettings() {
	if(!folderExists(OPT_DIR)) {
		CreateDirectory(OPT_DIR, NULL);
	}
	FILE *fd = fopen(OPT_FILE, "w+");
	char *buffer = (char *) malloc(42);
	sprintf(buffer, "%f\n%f\n", options.musicVolume, options.effectVolume); 
	if (fd != NULL) {
		fwrite(buffer, strlen(buffer), 1, fd);
	}
	free(buffer);
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
