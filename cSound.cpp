#include "cSound.h"

cSound::cSound() {
	nSounds = 0;
	FMOD::System_Create(&system);
	system->init(32, FMOD_INIT_NORMAL, 0);
}

cSound::~cSound(void)
{
}

int cSound::addSound(char *file, bool loop) {
	system->createSound(file, FMOD_HARDWARE, 0, &sounds[nSounds]);
	if(loop) sounds[nSounds]->setMode(FMOD_LOOP_NORMAL);
	else sounds[nSounds]->setMode(FMOD_LOOP_OFF);

	nSounds++;
	return nSounds-1;
}

void cSound::playSound(int id) {
	system->playSound(sounds[id], NULL, false, 0);
}

void cSound::stopSound(int id) {
	sounds[id]->release();
}

void cSound::resumeSound(int id) {
	playSound(id);
}

void cSound::pauseSound(int id) {
	system->playSound(sounds[id], NULL, true, 0);
}