#pragma once

#include "cScene.h"
#include "cData.h"
#include "cPlayer.h"
#include "cSound.h"
#include "utils.h"
#include <map>

#define	MAX_N_MONSTERS	10

#define TRANS_MAINMENU	7

#define NEW_GAME		0
#define OPTIONS			1
#define EXIT			2

#define MUSIC_VOLUME	0
#define EFFECTS_VOLUME	1
#define BACK			2

using namespace std;

class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	bool Init();
	bool startGame();
	bool Loop();
	void Finalize();
	void saveSettings();
	void loadSettings();

	//Input
	void ReadKeyboard(unsigned char key, bool press);
	void ReadSpecialKeyboard(unsigned char specialkey, bool press);
	void ReadMouse(int button, int state, int x, int y);
	//Process
	bool Process();
	//Output
	void Render();

private:
	unsigned char keys[256];
	unsigned char specialKeys[256];
	cScene Scene;
	cData Data;
	cPlayer Link;
	cSound sound;
	map<char,int> sounds;

	bool mainMenu;
	bool optMenu;
	int nTransMM;
	int currentMM;
	bool up;
	String menuText[3];
	String unLockedLevels[NUM_MAX_GATES];
	int currentOptMM;
	int currentPauseOpt;

	bool pause;

	bool mainMenuProcess();

	struct Options {
		float effectVolume;
		float musicVolume;
	} options;
};
