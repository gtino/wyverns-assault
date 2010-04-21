#include "..\include\Audio\AudioManager.h"

using namespace WyvernsAssault;

AudioManager::AudioManager()
{
	//
	// TODO Constructor
	//
}

AudioManager::~AudioManager()
{
	//
	// TODO Destructor
	//
	finalize();
}

/** Initialize the audio manager */
void AudioManager::initialize()
{
	//
	// TODO Initialize
	//
}

/** Finalize the audio manager */
void AudioManager::finalize()
{
	//
	// TODO Finalize
	//
}

void AudioManager::loadResources()
{
	//
	// TODO Load resources
	//
}

void AudioManager::unloadResources()
{
	//
	// TODO Unload resources
	//
}

// --------------------------------
// Lua Audio Lib
// --------------------------------
AudioManager* AudioManager::smAudioManager;

LUA_BEGIN_BINDING(AudioManager::audiolib)
LUA_BIND("playSound", AudioManager::playSound)
LUA_END_BINDING()

int AudioManager::playSound(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}