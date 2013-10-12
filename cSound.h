#pragma once

#include "fmod.hpp"
#include "fmod_errors.h"

#define MAX_SOUNDS	256

class cSound
{
public:
	cSound();
	~cSound(void);

	int addSound(char *file);
	void playSound(int id);
	void stopSound(int id);
	void resumeSound(int id);
	void pauseSound(int id);

private:
	FMOD::Sound *sounds[MAX_SOUNDS];
	int nSounds;
	FMOD::System *system;
};

