#pragma once

#include <Windows.h>
#include "Globals.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment( lib, "fmod_vc.lib" )

#define MAX_SOUNDS	256
#define MUSIC	0
#define EFFECT	1

using namespace FMOD;

class cSound
{
public:
	cSound();
	~cSound(void);

	int addSound(char *file, bool loop, int kind);	//kind=0 for MUSIC and =1 for EFFECTS
	void playSound(int id);
	void stopSound(int id);
	void resumeSound(int id);
	void pauseSound(int id);
	void updateSound();
	void setVolume(int idGroup, float vol);	// 0>=vol<=1, idGroup=0 for MUSIC and =1 for EFFECTS

private:
	System *system;
	Sound *sounds[MAX_SOUNDS];
	Channel *channels[MAX_SOUNDS];
	int musics[MAX_SOUNDS];
	int effects[MAX_SOUNDS];
	int nSounds;
	int nMusics;
	int nEffects;
	FILE *saveOpts;
	float effectVolume;
	float musicVolume;
};

