#include "..\..\include\Audio\Sound.h"

using namespace WyvernsAssault;

void SoundInstance::clear(void)
{
	fileName.clear();
	streamPtr.setNull();
	fileArchive = NULL;
	fmodSound = NULL;
	soundType = SOUND_TYPE_INVALID;
}

