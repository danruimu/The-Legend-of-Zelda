#pragma once

#include <Windows.h>
#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment( lib, "fmod_vc.lib" )

#define MAX_SOUNDS	256

using namespace FMOD;

class cSound
{
public:
	cSound();
	~cSound(void);

	int addSound(char *file, bool loop);
	void playSound(int id);
	void stopSound(int id);
	void resumeSound(int id);
	void pauseSound(int id);
	void updateSound();

private:
	System *system;
	Sound *sounds[MAX_SOUNDS];
	Channel *channels[MAX_SOUNDS];
	int nSounds;
};

