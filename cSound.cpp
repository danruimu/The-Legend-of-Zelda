#include "cSound.h"

cSound::cSound() {
	nSounds = 0;
	//FMOD::System_Create(&system);
}

cSound::~cSound(void)
{
}

int cSound::addSound(char *file) {
	//TODO: añadir sonido al vector de sounds

	nSounds++;
	return nSounds-1;
}

void cSound::playSound(int id) {

}

void stopSound(int id) {

}

void resumeSound(int id) {

}

void pauseSound(int id) {

}