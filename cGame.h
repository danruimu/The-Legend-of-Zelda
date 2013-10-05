#pragma once

#include "cScene.h"
#include "cData.h"
#include "cPlayer.h"

class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	bool Init();
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
};
