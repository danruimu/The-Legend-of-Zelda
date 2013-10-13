#include "cSound.h"

cSound::cSound() {
	nSounds = 0;
	FMOD::System_Create(&system);
	system->init(MAX_SOUNDS, FMOD_INIT_NORMAL, 0);
}

cSound::~cSound(void)
{
	for(int i = 0; i<nSounds; ++i) {
		sounds[i]->release();
	}
	system->release();
}

int cSound::addSound(char *file, bool loop) {
	if(nSounds==MAX_SOUNDS) return -1;
	system->createSound(file, FMOD_DEFAULT, 0, &sounds[nSounds]);
	if(loop) sounds[nSounds]->setMode(FMOD_LOOP_NORMAL);
	else sounds[nSounds]->setMode(FMOD_LOOP_OFF);

	this->pauseSound(nSounds);

	nSounds++;
	return nSounds-1;
}

void cSound::playSound(int id) {
	system->playSound(sounds[id], 0, true, &channels[id]);
	channels[id]->setPaused(false);
}

void cSound::stopSound(int id) {
	pauseSound(id);
}

void cSound::resumeSound(int id) {
	playSound(id);
}

void cSound::pauseSound(int id) {
	system->playSound(sounds[id], 0, true, &channels[id]);
	channels[id]->setPaused(true);
}