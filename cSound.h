#pragma once

#include <xaudio2.h>

class cSound
{
public:
	cSound();
	cSound(char* soundFile);
	~cSound(void);
	void playSound();

private:
	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasterVoice;
	IXAudio2SourceVoice* pSourceVoice;
};

