#pragma once

#include "fmod.h"
#include "fmod_codec.h"
#include "fmod.hpp"
#include "fmod_common.h"

class cSound
{
public:
	cSound();
	~cSound();
	void addFiles(char *file);
	
private:
	char *files[];
};

