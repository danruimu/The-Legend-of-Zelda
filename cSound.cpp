#include "cSound.h"

cSound::cSound() {
	nSounds = 0;
	nMusics = 0;
	nEffects = 0;
	System_Create(&system);
	system->init(MAX_SOUNDS, FMOD_INIT_NORMAL, 0);
}

cSound::~cSound(void)
{
	for(int i = 0; i<nSounds; ++i) {
		sounds[i]->release();
	}
	system->release();
}

int cSound::addSound(char *file, bool loop, int kind) {
	if(nSounds==MAX_SOUNDS) return -1;
	system->createSound(file, FMOD_HARDWARE, 0, &sounds[nSounds]);
	if(loop) sounds[nSounds]->setMode(FMOD_LOOP_NORMAL);
	else sounds[nSounds]->setMode(FMOD_LOOP_OFF);

	if (kind == MUSIC) {
		musics[nMusics] = nSounds;
		++nMusics;
	} else if(kind == EFFECT) {
		effects[nEffects] = nSounds;
		++nEffects;
	}

	//Start the sound paused to be able to set the volume
	system->playSound(sounds[nSounds], 0, true, &channels[nSounds]);

	nSounds++;
	return nSounds-1;
}

void cSound::playSound(int id) {
	if(id >= nSounds) return;
	bool play;
	channels[id]->isPlaying(&play);
	if(!play) system->playSound(sounds[id], 0, false, &channels[id]);
	else {
		bool paused;
		channels[id]->getPaused(&paused);
		if(paused) channels[id]->setPaused(false);
	}
}

void cSound::stopSound(int id) {
	if(id >= nSounds) return;
	bool play;
	channels[id]->isPlaying(&play);
	if(play) channels[id]->setPaused(true);
	channels[id]->setPosition(0, FMOD_TIMEUNIT_MS);
}

void cSound::resumeSound(int id) {
	if(id >= nSounds) return;
	bool play;
	channels[id]->isPlaying(&play);
	if(play) channels[id]->setPaused(false);
}

void cSound::pauseSound(int id) {
	if(id >= nSounds) return;
	bool play;
	channels[id]->isPlaying(&play);
	if(play) channels[id]->setPaused(true);
}

void cSound::updateSound() {
	system->update();
}

void cSound::setVolume(int idGroup, float vol) {
	if (vol < 0) vol = 0;
	else if (vol > 1) vol = 1;
	
	if(idGroup == MUSIC) {
		for(int i = 0; i<nMusics; ++i) {
			channels[musics[i]]->setVolume(vol);
		}
	} else if (idGroup == EFFECT) {
		for (int i = 0; i< nEffects; ++i) {
			channels[effects[i]]->setVolume(vol);
		}
	}
}
