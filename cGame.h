#pragma once

#include "cScene.h"
#include "cData.h"
#include "cPlayer.h"
#include "cSound.h"
#include <map>

#define	MAX_N_MONSTERS	10

#define SND_BACK	'a'
#define	SND_SW_ME	'b'
#define SND_SW_SH	'c'
#define SND_MAIN_MENU	'd'
#define SND_WHISTLE	'e'
#define SND_LINK_DIE 'f'

#define TRANS_MAINMENU	7

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
	cPlayer LinkSword;
	cBicho	monsters[MAX_N_MONSTERS];
	cSound sound;
	map<char,int> sounds;

	bool mainMenu;
	int nTransMM;
	int currentMM;
	char *menuText[3];
	int currentOptMM;
};
